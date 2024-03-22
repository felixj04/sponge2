Assignment 2 Writeup
=============

My name: [Jung Taewon ]

My POVIS ID: [taewon0727]

My student ID (numeric): [20220055]

This assignment took me about [12] hours to do (including the time on studying, designing, and writing the code).

If you used any part of best-submission codes, specify all the best-submission numbers that you used (e.g., 1, 2): [0, 1]

- **Caution**: If you have no idea about above best-submission item, please refer the Assignment PDF for detailed description.

Program Structure and Design of the TCPReceiver and wrap/unwrap routines:
1. wrapping_integer.cc
In the wrap function, we returns isn + n, which we should represent to 32 bit integer.
In the unwrap function, I made two variables, seq1 and seq2. 
In seq1, we should calculate the difference of n and isn in positive and negative.
And if seq1 is larger than checkpoint, we return it. 
If seq1 is smaller than checkpoint, we get the first 32bits from checkpoint and put
into seq1.
And, we repeat until seq1 is same or larger with checkpoint. 
seq2 is before last number of the upper loop.
Finally we compare seq1 and seq2 and return the closest one.
2. tcp_receiver
I first set initial states named is_syn and is_fin and set false.
i_seqno is the inital state for checking seqno.
After making header and assigned each variables, 
first I checked if syn and is_syn all 0, this case 
we should return nothing. After that case, change is_syn
to true. 
After, we find out if syn is 1 and is_syn is 0, whic means 
we got a new TCP connection.
In this case, we store the sequence number(seqno) and push
string into reassembler.
After, we check the fin code, if its finished, make eof 1.
Finally we push the data with index we find out. 
In ackno function, first we check if connection is connected.
If not return nothing. 
Finally it returns the next check num.

Implementation Challenges:
Using type casting was chellenging for me. 
It was confusing for me to know which type should I use. 
And finding out the variables in the skeleton code that which
variables are used and from where.

Remaining Bugs:
It has failure with ackno function. I think the ackno function returns 1 
in certain cases when it should not be return that number.

- Optional: I had unexpected difficulty with: Using the git branch. I cannot know if I submitted the right file. 

- Optional: I think you could make this assignment better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
