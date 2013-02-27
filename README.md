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

4. [MultiThreading](https://gist.github.com/1657289) support.

5. Variable flanking sequence length.

## Usage (count)

```
$ git clone git@github.com:drio/egrl.git
$ cd egrl && sh ./autogen.sh && ./configure && make && make install
$ egrl
```
Now let's say we have some illumina reads:

```
$ head reads.fata
>1
GGTTTACACTCTAGGTTACCGTGGGGGAAAGACTG
>2
GTTATCCAAGCTTCAGCAAAATATATCAGCATCAT
>3
GATTAATAACATGTTGATCTTTTTCTTCTTTCTGT
>4
GGTTTTCTAGAGGCAGAAGGCCAAGTTCATTCTCT
>5
GAGGGTTGATTCCAGAGAAGAACAAGTAGCCTCTC
```

And also we have some probes:

```
$ head probes.txt
1	100006955	rs4908018	TTTGTCTAAAACAAC	CTTTCACTAGGCTCA	C	A
1	100007331	rs2392072	TTATCATTCCCTTCC	GATCACCTCTACCAG	A	G
1	100008014	rs7541580	CTTCAGCTGAGAATG	TAAGGACCTGTGTGG	T	C
1	100008945	rs903127	GGCTGACAAAGACAC	GAGCTAGCAAATGAG	G	A
1	100016483	rs4907889	TGGGCTGGCCATAAA	CAATCAAAACCTCCT	C	T
1	100018883	rs12728909	GGGACTGAGGTAAGC	GTAGGGAAAGGTGGA	C	A
1	100024118	rs17121193	AAGTACACAATGTCC	AAGGCCTTTTTCATT	G	A
1	100024805	rs10875258	AGTGTAGTCCACCAA	CAGTAATATCAGCAT	T	C
1	100040066	rs12023333	GTTGCTTAACAATCC	TTCTTCAGTTCATCT	T	C
1	100040996	rs6688707	TTCAGGCCTTCAGTG	ATTTCCATGAGACCC	G	T
```

Probes is a tab delimited file that contains information of a particular
locus in the genome. The fields are: chromosome, coordinate, id, 5' flanking
sequence, 3' flanking sequence, expected reference allele and expected
variant allele.

Now we can use egrl to count how many times any of these probes
are being hit by our reads. I am assuming egrl is in your ```$PATH```

```
$ egrl count -p probes.affy.txt -r reads.fasta -t 8  > counts.csv
```

egrl dumps stuff to the stderr and the "counts" to the standard output:

```
$ head counts.csv
3,187080482,rs6809601,T,C,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0
9,83526867,rs1471723,G,C,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0
1,193412546,rs599140,T,C,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0
22,23680087,rs4140486,A,G,1,0,0,0,0,1,0,0,0,0,2,0,0,0,0
1,149638762,rs7172,G,A,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0
4,170686155,rs10010203,A,G,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0
3,187826181,rs13080283,G,A,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0
4,8985664,rs9291627,T,C,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0
13,103914442,rs7322187,G,A,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0
8,4567331,rs17346747,G,A,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0
```

The output is quite simple.A csv file with the chromosome,
coordinate, probe id, reference, variant and then the allele
counts from the reads. Only probes being hit are displayed.

The allele counts are separated in three parts: hits to
the ref allele, to the variant, to the alternative allele1
and the alternative allele2. The next five columns are the
same but from hits to the reverse complement of the probe.
The last five columns add the hits from the previous columns.

## Usage (hits)

There is another useful command (hits) that enumerates all the
hits to any of the probes. This command dumps directly to stdin
those hits, consisting in the probe id, the allele seen and the
read id that hits the probe.

This command uses much less memory since the hits are not stored
but reported immediately. The report of the read id per each
hit is also helpful in some projects.

Here you have a running example against a bunch of illumina reads.

```
$ egrl hits -p tests/probes.small.txt -r tests/reads.small.fq
>> Loading probes
[timer - Probe loading (SS)] wall clock: 0.23s CPU: 0.23s
>> # of probes (RC included): 40000
rs8067076,A,DCT4KXP1:297:C196LACXX:1:1101:3921:2216
rs2073067,G,DCT4KXP1:297:C196LACXX:1:1101:7679:2092
rs1065767,G,DCT4KXP1:297:C196LACXX:1:1101:8842:2157
rs10509305,A,DCT4KXP1:297:C196LACXX:1:1101:16274:2067
rs4844831,G,DCT4KXP1:297:C196LACXX:1:1101:7721:2569
rs7093194,G,DCT4KXP1:297:C196LACXX:1:1101:7698:2966
rs2272379,A,DCT4KXP1:297:C196LACXX:1:1101:9899:3023
[timer - count_main] wall clock: 0.46s CPU: 0.46s
```
