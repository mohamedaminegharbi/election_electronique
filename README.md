Election Management Application for Students (E-Election):

1. The program begins by asking for the number of administrators and voters (students).

2. Then, it creates a queue structure to store the information of administrators and students, and a stack structure to store the information of candidates, while keeping a copy of this information in two files throughout the program—one for everyone and one for the candidates.

3. The program then enters a loop where it asks each administrator and voter to enter their information. If a person chooses to run as a candidate, they are added to the stack of candidates and recorded in the corresponding file.

4. After collecting all the information, the program enters the voting process. Each voter is prompted to enter their ID number and select a candidate to vote for.

5. If an administrator enters their ID number during the voting process, they have the option to stop the election by entering 0 (for example, in the case where the election duration has passed; in this case, the administrator must stop the election even if not all students have voted).

6. Finally, the program counts the votes and determines the winning candidate, then records their information in a file.
