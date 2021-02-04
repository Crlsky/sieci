#include <iostream>
#include <conio.h>
using namespace std;
int main()
{
    long long zmienna = 213;
    long long * wskaznik =& zmienna;
    cout << "&zmienna=" <<& zmienna << endl;
    cout << "wskaznik=" << wskaznik << endl;
    getch();
    return( 0 );
}