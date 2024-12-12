#include <iostream>
#include <fstream>

using namespace std;

const int MAX_ROWS = 10;
const int MAX_COLS = 10;

class MatrixType {
    private:
        int values[MAX_ROWS][MAX_COLS];
        int numRows;
        int numCols;
    public:
        MatrixType();
        void MakeEmpty();
        void setSize(int rowsSize,int colSize);
        void storeItem(int item, int row, int col);
        void Add(MatrixType otherOperand, MatrixType& result);
        void Sub(MatrixType otherOperand, MatrixType& result);
        void Mult(MatrixType otherOperand, MatrixType& result);
        void Print(ofstream& outfile);
        bool AddSubCompatible(MatrixType otherOperand);
        bool MultCompatible(MatrixType otherOperand);
        void PrintToConsole() {
            for (int i = 0; i < numRows; i++) {
                for (int j = 0; j < numCols; j++) {
                    cout << values[i][j] << " ";
                }
                    cout << endl;
            }
        }
};
MatrixType::MatrixType() {
    //pre: none
    //post: Initializes a MatrixType object with numRows and numCols 
    // set to 0, and all elements in the values array set to 0
    numRows = 0;
    numCols = 0;
    
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++) {
            values[i][j] = 0;
        }
    }

}
void MatrixType::MakeEmpty() {
    //pre: none
    //post: sets all values in matrix to 0
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            values[i][j] = 0;
        }
    }
}

void MatrixType::setSize(int rowsSize, int colsSize) {
    //pre: row and cols must be non negative and not more than 10
    //post: sets matrix to specified size otherwise prints error
    if (rowsSize <= MAX_ROWS && colsSize <= MAX_COLS && rowsSize >= 0 && colsSize >= 0) {
        numRows = rowsSize;
        numCols = colsSize;
    } else
        cerr << "\nrows or cols exceeded limit/invalid\n";
}

void MatrixType::storeItem(int item, int row, int col) {
    //pre: row and col must be valid dimensions
    //post: inserts value into specified position in the matrix 
    if (row >= 0 && row < numRows && col >= 0 && col < numCols)
        values[row][col] = item; 
}

void MatrixType::Add(MatrixType otherOperand, MatrixType& result) {
    //pre: current and otherOperand matrix must be same dimension
    //post: performs operation if condition is met. 
    //intializes result and stores the result of add/sub into result othewise prints error
    if (AddSubCompatible(otherOperand)) {
        result.setSize(numRows,numCols); // intialize the result
        result.MakeEmpty();
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                result.values[i][j] = values[i][j] + otherOperand.values[i][j];
            }
        }
    }
    else 
        cerr << "Error: Matrices must be equal in dimensions\n"; 
}

void MatrixType::Sub(MatrixType otherOperand, MatrixType& result) {
    if (AddSubCompatible(otherOperand)) {
        result.setSize(numRows, numCols);
        result.MakeEmpty();
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                result.values[i][j] = values[i][j] - otherOperand.values[i][j];
            }
        }
    }
    else 
        cerr << "Error: Matrices must be equal in dimensions\n";
}

void MatrixType::Mult(MatrixType otherOperand, MatrixType& result) {
    //pre: current matrix cols must equal otherOperand rows
    //post: performs multiplication, intializes result otherwise print error
    if (MultCompatible(otherOperand)) {
        result.setSize(numRows, otherOperand.numCols); // ex. 5x2 2x5 the result would be a 5x5 matrix.
        result.MakeEmpty();
        // Loop through rows of the current matrix and columns of the other matrix
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < otherOperand.numCols; j++) {
                // Multiply and accumulate elements
                for (int k = 0; k < numCols; k++) {
                    result.values[i][j] += values[i][k] * otherOperand.values[k][j];
                }
            }
        }
    }
    else 
        cerr << "Error: Matrices are not compatible for multiplication.\n";
}

void MatrixType::Print(ofstream& outFile) {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            outFile << values[i][j] << " ";
        }
        outFile << endl;
    }
}

bool MatrixType::AddSubCompatible(MatrixType otherOperand) {
    //matrices must be equal in dimensions to be compatible for Add/Sub
    return (numRows == otherOperand.numRows && numCols == otherOperand.numCols);
}

bool MatrixType::MultCompatible(MatrixType otherOperand) {
    //the first matrix cols must equal the second matrix rows inorder for them to be
    //compatible
    return (numCols == otherOperand.numRows);
}

enum CommandChoice {
    GetNewMatrix,
    AddMatrices,
    SubMatrices,
    MultiplyMatrices,
    PrintMatrix,
    Quit
};

int main()
{
    MatrixType matrices[10]; // create an array of MatrixType
    int index1, index2, resultIndex; // for accessing MatrixType arrays
    ofstream outFile;
    //open file for writing
    outFile.open("output.txt");
    if(!outFile.is_open()) {
        cerr << "Error opening file"; 
        return -1;
    }
    while(true){
        cout << "Menu:\n";
        cout << "0.GetNewMatrix\n1.AddMatrices\n2.SubMatrices\n3.MulMatrices\n4.PrintMatrix\n5.Quit\n";
        cout << "Enter choice #(0-5): ";
        int input;
        cin >> input;
        // input validation
        if (cin.fail()) {
            cerr << "Invalid input. Please enter a valid integer.\n";
            outFile << "Invalid input\n";
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore the rest of the line
            continue; // Go back to the start of the loop
        }
      
        CommandChoice command;
        command = static_cast<CommandChoice>(input);  
        
        switch (command) {
            case GetNewMatrix:
                outFile << "\nEntered Choice: GetNewMatrix\n";
                int matrixIndex;
                cout << "Enter Matrix Index (0-9): ";
                cin >> matrixIndex;
                if (matrixIndex >= 0 && matrixIndex < 10) { // validate range
                    int rows,cols;
                    cout << "Enter Matrix " << matrixIndex << " rows: ";
                    cin >> rows;
                    cout << "Enter Matrix " << matrixIndex << " cols: ";
                    cin >> cols;
                    if (rows <= 0 || cols <= 0 || rows > 10 || cols > 10) {
                        cerr << "Invalid matrix dimensions. Rows and columns must be greater than zero and less than 11.\n";
                        outFile << "Invalid matrix dimensions\n";
                        continue;
                    }

                    outFile << "\nMatrix " << matrixIndex << " rows: " << rows;
                    outFile << "\nMatrix " << matrixIndex << " columns: " << cols;

                    matrices[matrixIndex].setSize(rows,cols);
                    matrices[matrixIndex].MakeEmpty();
                    for (int i = 0; i < rows; i++) {
                        for (int j = 0; j < cols; j++) {
                            cout << "Enter value for matrix" << matrixIndex << "[" << i << "][" << j << "]: ";
                            int val;
                            cin >> val;
                            matrices[matrixIndex].storeItem(val, i, j);
                        }
                    }
                    outFile << endl;
                    matrices[matrixIndex].Print(outFile);
                }
                else {
                    cerr << "Invalid matrix index. Enter (0-9).\n";
                    outFile << "Invalid matrix index\n";
                }
                cout << "\n GetNewMatrix Finished\n\n";
                break;
           
            case AddMatrices:
                // Prompt the user for the indices of the matrices to add
                outFile << "\nEntered Choice: AddMatrices\n";
                cout << "Enter index of first Matrix(0-9):";
                cin >> index1;
                cout << "Enter index of second Matrix(0-9:";
                cin >> index2;
                // Prompt the user for the index where the result will be stored
                cout << "Enter index of result Matrix(0-9):";
                cin >> resultIndex;
                if (index1 >= 0 && index1 < 10 && index2 >= 0 && index2 < 10 && resultIndex >= 0 && resultIndex < 10) {
                    // Access the matrices using the provided indices
                    matrices[index1].Add(matrices[index2], matrices[resultIndex]);
                    // Check if compatible for Addition
                    if (matrices[index1].AddSubCompatible(matrices[index2])) 
                        cout << "\nMatrices Added\n\n";
                    else 
                        outFile << "\nError: Matrices dimension incompatible for Addition\n";
                }
                else {
                    cout << "Invalid matrix indices. Please enter valid indices between 0 and 9.\n";
                }
                break;

            case SubMatrices:
                outFile << "\nEntered Choice: SubMatrices\n";
                cout << "Enter index of first Matrix(0-9):";
                cin >> index1;
                cout << "Enter index of second Matrix(0-9:";
                cin >> index2;
                cout << "Enter index of result Matrix(0-9):";
                cin >> resultIndex;
                if (index1 >= 0 && index1 < 10 && index2 >= 0 && index2 < 10 && resultIndex >= 0 && resultIndex < 10) {
                    // Access the matrices using the provided indices
                    matrices[index1].Sub(matrices[index2], matrices[resultIndex]);
                    if (matrices[index1].AddSubCompatible(matrices[index2]))
                        cout << "\nMatrices Subtracted\n\n";
                    else
                        outFile << "\nError: Matrices dimension incompatible for Subtraction\n";
                }
                else {
                    cout << "Invalid matrix indices. Please enter valid indices between 0 and 9.\n";
                }
                break;

            case MultiplyMatrices:
                outFile << "\nEntered Choice: MultiplyMatrices\n";
                cout << "Enter index of first Matrix(0-9):";
                cin >> index1;
                cout << "Enter index of second Matrix(0-9:";
                cin >> index2;
                cout << "Enter index of result Matrix(0-9):";
                cin >> resultIndex;
                if (index1 >= 0 && index1 < 10 && index2 >= 0 && index2 < 10 && resultIndex >= 0 && resultIndex < 10) {
                    // Access the matrices using the provided indices
                    matrices[index1].Mult(matrices[index2], matrices[resultIndex]);
                    if (matrices[index1].MultCompatible(matrices[index2]))
                        cout << "\nMatrices Multiplied\n\n";
                    else
                        outFile << "\nError: Matrices dimension incompatible for Multiplication\n";
                }
                else {
                    cout << "Invalid matrix indices. Please enter valid indices between 0 and 9.\n";
                }
                break;

            case PrintMatrix:
                outFile << "\nEntered Choice: PrintMatrix\n";
                int printIndex;
                cout << "Enter the index of the matrix to print: ";
                cin >> printIndex;

                // Check if the entered index is valid
                if (printIndex >= 0 && printIndex < 10) {
                    // Print the specified matrix
                    matrices[printIndex].Print(outFile);
                    matrices[printIndex].PrintToConsole();
                    cout << "\nMatrix Printed\n\n";
                }
                else {
                    cout << "Invalid matrix index. Please enter a valid index (0-9).\n";
                    outFile << "Invalid matrix index\n";
                }
                break;
                
            case Quit:
                outFile << "\nEntered Choice: Quit...End of Outputs\n";
                return 0;

            default:
                cout << "Invalid command. Please enter a valid command.\n";
        }
    }
    outFile.close();
    return 0;
}

