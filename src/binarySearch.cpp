#include "binarySearch.h"

int binarySearch(int arr[], int low, int high, int x) {
    while (low <= high) {
        int mid = low + (high - low) / 2;

        // If the middle element is equal to 'x', we have 
        // found the element, return its index 
        if (arr[mid] == x)
            return mid;

        // If the middle element is smaller than 'x', search 
        // in the right half of the array 
        if (arr[mid] < x)
            low = mid + 1;

        // If the middle element is greater than 'x', search 
        // in the left half of the array 
        else
            high = mid - 1;
    }

    // If the base case is reached, the element is not 
    // present in the array, return -1 
    return -1;
}
