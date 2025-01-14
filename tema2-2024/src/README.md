# Rusanescu Andrei-Marian 313CC

Task 1:

This task's purpose is to find if the ant with the id of i
can get their desired rooms. The id and rooms are stored
in the bits of an integer, n. The id is stored in the most
significant 8 bits (i.e. msb: 00000100 => id 4).

To solve this task, first the value of n is separated from
the id. In register EDX, it is stored the address of ant[i].
The value of ant[i] is then stored in EAX. Iterating from 0
to 23, every 1 - bit from n is compared with the corresponding
bit from ant[i]. If both are 1 then the procedure continues,
otherwise it means the room is occupied so the ant cannot
reserve it. In the end, if the ant got all the rooms it
wanted, the value of res is made 1, otherwise 0.


Task 2

Task 2 is made out of 2 parts, first to sort an array
of login requests, then, based on that sorted array, to check
if a certain passkey is associated with a hacker or not.

Task 2.1:

The array is sorted using 2 loops with 3 conditions, in this
exact order:
    - condition 1: if the user is admin
    - condition 2: check the priority of the user
    - condition 3: compare the strings associated with their
                   username lexicographically

At the end, if any of the conditions above was met, the
login requests are swapped byte by byte.

Condition 1:
If one user is not admin and the one after him is, then
their requests are swapped.

Condition 2 (both users are admin):
If one user's priority is lower than the one after, their
requests are swapped.

Condition 3 (both are admin and have the same priority):
If one user's username is lexicographically higher than the
one after him, their requests are swapped.


Task 2.2:

A passkey is associated with a hacker if the MSB and the LSB
are both set to 1 and for the most significant 7 bits after
MSB there is an odd number of 1 - bits, and for the least
significant 7 bits after LSB, ther is an even number of 1
bits.

In the present implementation, first the basecases are
checked, meaning that if the MSB and LSB are set to 1.
If these conditions are met, then every remaining bit is
checked. After checking the least 7 significant remaining bits
if their number is even, the flag (ESI) is set to 1 so it is 
clear where the checking got. Afterwards, the checking process
continues until the end. If the most significant 7 bits are in
an odd number, then the passkey is associated with a hacker.
If any of the conditions above were not met, the checking
stops, the passkey not being associated with a hacker.


Task 3:

This task's objective is to implement treyfer encyption /
decryption.
The key features of the implementation are that the remainder
of the division by 8 was calculated using x & 7. This only
works for division by powers of 2 (x & (n - 1)).
Encryption was done in one loop, due to both indices starting
from 0, second one goes until 8, and the first one until 10.


Task 4

The given task's purpose is to solve a labyrinth represented
by a matrix of m rows and n columns. A wall is a value of '1',
whilst a pass is a value of '0'. There is only one correct way
to solve the labyrinth. In order not to go back, every
previous position is set to '1'.

There are 4 cases a person could go: right, left, up, down.
In order to access the value of labyrinth[i][j], i.e.
*(*(labyrinth + i) + j), it is needed to calculate the address
of the pointer to the row i, then the address of column j in
that row. To do that, knowing the size of any pointer on a
32 - bit system is 4 bytes, i is multiplied by 4 and then
added to the address of the labyrinth. This address is then
dereferenced. To it, it is added j + sizeof(char), i.e. j.
This address is then dereferenced and the result is the value
of labyrinth[i][j].
While i and j are not m - 1, n - 1 respectively, the 4 cases
are checked, i or j is incremented / decremented. At the
end, the values of the last indices are the solution.
