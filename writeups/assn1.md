Assignment 1 Writeup
=============

My name: Taewon Jung

My POVIS ID: taewon0727

My student ID (numeric): 20220055

This assignment took me about 13 hours to do (including the time on studying, designing, and writing the code).

Program Structure and Design of the StreamReassembler:
1. setting variables.
The variable initial_loc holds the first place we need to find.
It can be calculated by bytes_written() by output, which we had already rearranged it.
Last_loc is the variable that we should end in that part. It can be calculated by adding initial_loc, capacity and buffer size.
num_assembled is the variable that counts the number of data arranged. 
In the private member, we have _check, _buffer and num_unassembled.
_check is string that have the data that its data had rearranged or not.
_buffer is a string that we helds the data before sending it into output.
num_unassembled is the variable holds the number of unrearranged data. 

2. first loop 
This program first checks the length of the size that we can replace.
The variable initla_loc helds that opportunity.
This holds the location of the first place, which is represented as bytes
written by previous operations. 
The end location of the place that we want to change is at the variable last_loc
 After this position we need to throw away that part.
After checking the boundary, we need to find out which string has rearranged or not.
In order to do this, I made a private member named _check and _buffer.
The first one is a string that holds whether it has rearranged or not.
Therefore, it is first initialzed as 0.
_buffer is the string private member that holds data pushed from push_substring.
It stores until it goes to the output part. So in the first loop code, it first
checks if the string had rearranged or not.
After that, it rearranges its place using index and changes check into
1 and increases num_unassembled.

3. second loop
 In the second loop, it finds out the number of assembled.
Since, the max size that we have in this class is _capacity, the size of the loop is _capacity.
This part simply checks whether it is arranged or not and counts the arranged part.
After that, we should move the string from buffer to output.
The following 5 lines performs that.
First line moves lines from buffer in amount of number arranged.
And we delete that part and refill it with blank in order to maintain the size of the buffer.
And reset that location in check to 0 by deleting and re-appending it.

4. checking eof condition
 Finally we need to check the condition of eof.
It has three conditions, check the eof that we got from the function and check if
the data is within our size by comparing with last_loc.
Also it checks whether we rearranged all the data.
If the three conditions are all true, it calls end_input and finishes it.
The other two functions returnes number of rearranged data and if the data is all rearranged. 
 
Implementation Challenges:
It was hard to handle execption conditions.
I have to try a lot of time passing the following tests.
I first counted num_assembled in the first loop.
I thought doing the first loop means number of assembled data had increased.
But I was wrong. Therefore I need to make a second loop that checks the number of assembled data itself.
The second was considering the range in the first loop. I first considered only the initial_loc and last_loc.
It was a mistake and I thought I need to consider about the index too.
Also finding out the location in the string and matching it was challenging to me.
I need to solve this by drawing the array and find the right place.
The third hard thing was to think other execption condition that I need to consider more.
First I put extra conditions like finding out whether max(initial_loc, index) is
smaller than  min(last_loc, index + data.length()) or not.
But since I found out that the passed test case didn't changed, I removed that conditon.
Like that, it was challenging to find which condition is neccesary.
The last problem is handling eof.
I cannot pass 2 testcases because of its eof exceptions.
And cannot find about that reason.   

Remaining Bugs:
As I wrote it in the second place, this code have problem in eof conditions.
The two testcases that I didn't pass was all about eof. In the 4th testcase, it should return eof but doesn't.
I thought the condition that figures out index + data.length() and last_loc which is bigger.
But, it wasn't. After I changed this condition it still made an error.
The eof condition should be right, so I didn't handle that part.
But after handling other two parts, it makes additional errors than before.
Therefore I gave up handling that part.  

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this assignment better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
