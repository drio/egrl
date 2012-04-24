# Intro

Egrl is a fresh implementation of e-genotype, an allele screening method for
[Next-Gen sequencing data](http://en.wikipedia.org/wiki/DNA_sequencing).

## What's new

Here's some of the new features:

1. probes and reads can be feed in the stdin. Yes, who doesn't love Unix?
This also allows the processing of data from
[bam/sam](http://samtools.sourceforge.net/SAM1.pdf).

2. Fasta(q) allowed.

3. Processes compressed files (gzip).

4. Transparent CS support. The user doesn't have to generate probes in CS.
Probes can be fed in sequence space and the tool will automatically
generate the CS version while processing the input data.

5. [https://gist.github.com/1657289](Multithreading) support.

## Usage

```
$ git clone git@github.com:drio/egrl.git
$ sh ./autogen.sh && ./configure && make && make install
$ egrl
```

