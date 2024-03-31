#include "syscall.h"

int main()
{
    int a[100]; 
    int n;         
    int order;       // kieu sort

    /* Quicksort */

    int stack[100]; //stack de luu tru left va right 
    int top;             // Top cua the stack
    int left;            // Left index of the sub array to be sorted
    int right;           // Right index of the sub array to be sorted
    int pivotIndex;      // Index of the pivot element
    int pivot;           // Pivot element
    int i, j;            // Loop variables
    int temp;            // Temporary variable for swapping

    /* File handling */
    int openFileId; // File descriptor for the file to be written

    top = -1;
    left = 0;

    // Read n, the number of elements in the array
    do
    {
        PrintString("Enter n (1 <= n < 100): ");
        n = ReadInt();
        if (n < 0 || n > 100)
            PrintString("n phai la so nguyen < 100\n");
    } while (n < 0 || n > 100);

    right = n - 1;
    i = 0;

    // Read the elements of the array
    for (i; i < n; i++)
    {
        PrintString("Phan tu thu [");
        PrintInt(i);
        PrintString("]: ");
        a[i] = ReadInt();
    }

    // Read the order of sorting
    do
    {
        PrintString("Loai sort (1: increasing, 2: decreasing): ");
        order = ReadInt();
        if (order != 1 && order != 2)
            PrintString("Wrong input, please try again\n");
    } while (order != 1 && order != 2);

    // Quick sort
    stack[++top] = left;
    stack[++top] = right;

    while (top >= 0)
    {
        right = stack[top--];
        left = stack[top--];

        pivotIndex = left + (right - left) / 2;
        pivot = a[pivotIndex];

        i = left, j = right;

        while (i <= j)
        {
            while (a[i] < pivot)
                i++;
            while (a[j] > pivot)
                j--;

            if (i <= j)
            {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
                i++;
                j--;
            }
        }

        if (left < j)
        {
            stack[++top] = left;
            stack[++top] = j;
        }

        if (i < right)
        {
            stack[++top] = i;
            stack[++top] = right;
        }
    }

    // Reverse the array if order is decreasing
    if (order == 2)
    {
        for (i = 0; i < n / 2; i++)
        {
            temp = a[i];
            a[i] = a[n - i - 1];
            a[n - i - 1] = temp;
        }
    }

    // Print the sorted array to console
    PrintString("Sorted array: ");
    for (i = 0; i < n; i++)
    {
        PrintInt(a[i]);
        PrintChar(' ');
    }

    // Open the file to write the sorted array
    openFileId = Open("quicksort.txt", 0);

    // Write the sorted array to file
    if (openFileId != -1)
    {
        for (i = 0; i < n; i++)
        {
            Write(&a[i], 4, openFileId);
        }
        Close(openFileId);
        return 0;
    }
    else
    {
        PrintString("Can't open file\n");
        return 1;
    }
}
