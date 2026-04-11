#!/bin/bash
bedtools slop -i res/NA_summits.bed -g genome.size -b 100 > summits_extended.bed
bed2fasta -o sequences.fa summits_extended.bed to_use/GCA_000001405.29_GRCh38.p14_genomic.fna
meme sequences.fa -dna -revcomp -mod oops -nmotifs 1 -w 19
tomtom -oc tomtom_out -no-ssc -verbosity 1 -min-overlap 5 -dist pearson -thresh 0.05 meme_out/meme.txt to_use/JASPAR2026.txt
