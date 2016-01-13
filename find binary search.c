int half_seek ( int arr[], int size, int v )
{
    int left = 0;
    int right = size - 1;
    int mid = 0;

    while ( left < right )
    {
        mid = ( left + right ) / 2;

        if ( arr[mid] > v )
        {
            right = mid - 1;
        }
        else if ( arr[mid] < v )
        {
            left = mid + 1;
        }
        else
        {
            return mid;
        }
    }

    return -1;
}