# Lab 3 — Wildcards and Links

## 1. Wildcards

Wildcards are used to match filenames in Linux.

- `*` matches zero or more characters. For example, `practice/*.txt` matches all `.txt` files in the practice folder.
- `?` matches exactly one character. For example, `practice/report?.txt` matches `report1.txt` and `report2.txt`.
- When a wildcard is inside single quotes, the shell treats it as literal text instead of expanding it.

## 2. Hard Links and Symbolic Links

A hard link is another name for the same file because it shares the same inode.

A symbolic link is a shortcut that stores the path to another file. It has its own inode.

From my experiment:
- `source.txt` and `hard.txt` shared inode `18743628`.
- `soft.txt` had a different inode, `18745272`.

## 3. Predictions and Results

### Prediction 1: Renaming the Original File

**Original prediction:**

1. I thought the hard link would still read the original content because only the filename changes.
2. I thought the symbolic link would stop working because its stored path would no longer match the filename.

**Result: Confirmed**

After renaming `source.txt` to `renamed.txt`, `hard.txt` still displayed `version 1` because it shared the same inode as the original file.

However, `soft.txt` stopped working because it still pointed to `source.txt`. After recreating the symbolic link to point to `renamed.txt`, it worked again.

### Prediction 2: Creating a New Source File

**Original prediction:**

I thought `hard.txt` would still display version 1 because it was connected to the original inode.

**Result: Confirmed**

After creating a new `source.txt`, it displayed `version 2`, while `hard.txt` still displayed `version 1`.

The new `source.txt` had inode `18745282`, while `hard.txt` kept inode `18743628`. This shows that creating a new file with the same name does not reuse the original inode.

## 4. Evidence

- `evidence/links.txt` contains the link experiment, symbolic-link failure, and repair results.
- `evidence/replacement.txt` contains the inode numbers and file contents after creating a new source file.

## 5. What I Learned

I learned how wildcards work and how to create hard links and symbolic links in Linux. I also learned that hard links share the same inode, while symbolic links store a path. Renaming a file can break a symbolic link, but the hard link still works because it points to the same inode.
