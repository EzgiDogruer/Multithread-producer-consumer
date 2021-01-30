# Multithread-producer-consumer

In this assignment, you will write a program that will package books published by different publishers.
You will have publisher and packager threads. After publishers publish books, packagers package the
books published.

Sample output :
o Example:
<Thread-type and ID> <Output>
Publisher 1 of type 1 Book1_1 is published and put into the buffer 1.
Publisher 2 of type 2 Book2_1 is published and put into the buffer 2.
Publisher 2 of type 2 Book2_2 is published and put into the buffer 2.
Packager 1 Put Book2_1 into the package.
Packager 2 Put Book2_2 into the package.
Packager 1 Put Book1_1 into the package.
............................................................................................................................
Publisher 2 of type 1 Finished publishing 5 books. Exiting the system.
............................................................................................................................
Packeger 4 Finished preparing one package. The package contains:
Book2_3, Book2_7 .... Book1_13.
Publisher 3 of type 1 Buffer is full. Resizing the buffer.
.............................................................................................................................
Packager 3 There are no publishers left in the system. Only 1 of 6
number of books could be packaged. The package
contains Book2_15. Exiting the system.
