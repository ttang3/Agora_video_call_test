package main

import (
    "log"
    "fmt"
    "bytes"
	"strconv"
    "net/http"

    bolt "go.etcd.io/bbolt"
)

type boltDB struct {
    db_name   				string
	bucket_name				[]string
	numOfBucket				int
	currentBucketIndex		int
    db        				*bolt.DB
	bucket  				[]*bolt.Bucket
	err		  				error
}

const (
	NUMOFBUCKET  = 		10
	KVDBNAME = 		"my.db"
	MYBUCKETNAME = 		"mybucket"
)

var myBoltDB		*boltDB

func initkv() error {
	myBoltDB = &boltDB {
		db_name: 				KVDBNAME,
		bucket_name:         	make([]string, NUMOFBUCKET),
    	numOfBucket:        	NUMOFBUCKET,
    	currentBucketIndex:	0,
    	db:						nil,
    	bucket:                 make([]*bolt.Bucket, NUMOFBUCKET),
    	err:                    nil,
	}

	err := myBoltDB.open()
	if err != nil   {
		return err
	}
	

	return  myBoltDB.createBucket(0, MYBUCKETNAME)
}

func (c *boltDB) open() error {
    c.db, c.err = bolt.Open(c.db_name, 0600, nil)
    if c.err != nil {
    	log.Fatal(c.err)
	}
	return c.err
}

func (c *boltDB) close() {
	c.db.Close()
}

func (c *boltDB) createBucket(index int, bucketname string) error {
	c.bucket_name[index] = bucketname
		fmt.Printf("start in createBucket(), bucketname = %s\n", bucketname)
	c.db.Update(func(tx *bolt.Tx) error {
		fmt.Printf("before start in createBucket()\n")
		c.bucket[index], c.err = tx.CreateBucketIfNotExists([]byte(c.bucket_name[index]))
		fmt.Printf("after start in createBucket()\n")
		if c.err != nil {
		fmt.Printf("error start in createBucket()\n")
			return fmt.Errorf("create bucket: %s", c.err)
		}
		return nil
	})		
	c.currentBucketIndex = index	
	return c.err
}

func (c *boltDB) deleteBucket(index int) {
	c.db.Update(func(tx *bolt.Tx) error {
		c.err = tx.DeleteBucket([]byte(c.bucket_name[index]))
		if c.err != nil {
			return fmt.Errorf("delete bucket: %s", c.err)
		}
		return nil
	})		
	c.bucket[index] = nil
}

func (c *boltDB) changeBucket(index int) {
	c.currentBucketIndex = index;	
}

func (c *boltDB) put(key []byte, value []byte) error {
	c.db.Update(func(tx *bolt.Tx) error {
		fmt.Printf("before start in put(), key = %v, value = %v\n", key, value)
		b := tx.Bucket([]byte(c.bucket_name[c.currentBucketIndex]))
		c.err = b.Put(key, value)
		fmt.Printf("after start in put(), err = %v\n", c.err)
		return c.err
	})
	return c.err
}

func (c *boltDB) delete(key []byte) error {
	c.db.Update(func(tx *bolt.Tx) error {
		b := tx.Bucket([]byte(c.bucket_name[c.currentBucketIndex]))
		c.err = b.Delete(key)
		return c.err
	})
	return c.err
}

func (c *boltDB) get(key []byte, w http.ResponseWriter) {
		fmt.Printf("start in get()\n")
	c.err = c.db.View(func(tx *bolt.Tx) error {
		fmt.Printf("before start in get()\n")
		b := tx.Bucket([]byte(c.bucket_name[c.currentBucketIndex]))
		v := b.Get(key)
		fmt.Printf("after start in get()\n")
		if v != nil {
			fmt.Printf("The answer is: %s\n", v)
			w.Write(v)
			fmt.Printf("after Write in get()\n")
		}
		return nil
	})
}

func (c *boltDB) forEach(filename string, w http.ResponseWriter) {
	contentdisp := "attachment; filename=" + filename
fmt.Printf("contentdisp = %v\n", contentdisp)
	w.Header().Set("Content-Type", "application/octet-stream")
	w.Header().Set("Content-Disposition", contentdisp)
	//w.Header().Set("Content-Length", strconv.Itoa(int(tx.Size())))
	c.db.View(func(tx *bolt.Tx) error {
		// Assume bucket exists and has keys
		b := tx.Bucket([]byte(c.bucket_name[c.currentBucketIndex]))
		b.ForEach(func(k, v []byte) error {
			w.Write(v)
			fmt.Printf("key=%s, value=%s\n", k, v)
			return nil
		})
		return nil
	})
}

func (c *boltDB) prefixGet(prefix []byte, w http.ResponseWriter) {
	c.db.View(func(tx *bolt.Tx) error {
		b := tx.Bucket([]byte(c.bucket_name[c.currentBucketIndex]))
		cur := b.Cursor()
		// Assume bucket exists and has keys
		for k, v := cur.Seek(prefix); k != nil && bytes.HasPrefix(k, prefix); k, v = cur.Next() {
			fmt.Printf("key=%s, value=%s\n", k, v)
			w.Write(v)
		}
		return nil
	})
}

func (c *boltDB) containGet(containStr []byte, w http.ResponseWriter) {
    c.db.View(func(tx *bolt.Tx) error {
        b := tx.Bucket([]byte(c.bucket_name[c.currentBucketIndex]))
        cur := b.Cursor()
        // Assume bucket exists and has keys
        for k, v := cur.Seek(containStr); k != nil && bytes.Contains(k, containStr) == true; k, v = cur.Next() {
            fmt.Printf("key=%s, value=%s\n", k, v)
            w.Write(v)
        }
        return nil
    })
}

func (c *boltDB) rangeGet(min, max []byte, w http.ResponseWriter) {
	c.db.View(func(tx *bolt.Tx) error {
		b := tx.Bucket([]byte(c.bucket_name[c.currentBucketIndex]))
		cur := b.Cursor()
		// Assume bucket exists and has keys
		for k, v := cur.Seek(min); k != nil && bytes.Compare(k, max) <= 0; k, v = cur.Next() {
			if v != nil {
				fmt.Printf("key=%s, value=%s\n", k, v)
				w.Write(v)
			}
		}
		return nil
	})
}

func (c *boltDB) backup(filename string, w http.ResponseWriter) error {
	contentdisp := "attachment; filename=" + filename
	err := c.db.View(func(tx *bolt.Tx) error {
		w.Header().Set("Content-Type", "application/octet-stream")
		w.Header().Set("Content-Disposition",contentdisp) 
		w.Header().Set("Content-Length", strconv.Itoa(int(tx.Size())))
		_, err := tx.WriteTo(w)
		return err
	})
	return err;
}
