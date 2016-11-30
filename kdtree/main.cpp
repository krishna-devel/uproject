#include "tbb/tbb.h"
#include <iostream>
using namespace tbb;
using namespace std;

class first_task : public task {
public:
    task* execute( ) {
        cout << "Hello World!\n";
        return NULL;
    }
};

int main( )
{
    task_scheduler_init init(task_scheduler_init::automatic);
    first_task& f1 = *new(tbb::task::allocate_root()) first_task( );
    tbb::task::spawn_root_and_wait(f1);
}

//#include <stdio.h>
//#include <string.h>  // for strlen
//#include <assert.h>
//#include "zlib.h"
//#include <getopt.h>
//
//// adapted from: http://stackoverflow.com/questions/7540259/deflate-and-inflate-zlib-h-in-c
//int main(int argc, char* argv[])
//{
//    // original string len = 36
//    char a[50] = "Hello Hello Hello Hello Hello Hello!";
//
//    // placeholder for the compressed (deflated) version of "a"
//    char b[50];
//
//    // placeholder for the UNcompressed (inflated) version of "b"
//    char c[50];
//
//
//    printf("Uncompressed size is: %lu\n", strlen(a));
//    printf("Uncompressed string is: %s\n", a);
//
//
//    printf("\n----------\n\n");
//
//    // STEP 1.
//    // deflate a into b. (that is, compress a into b)
//
//    // zlib struct
//    z_stream defstream;
//    defstream.zalloc = Z_NULL;
//    defstream.zfree = Z_NULL;
//    defstream.opaque = Z_NULL;
//    // setup "a" as the input and "b" as the compressed output
//    defstream.avail_in = (uInt)strlen(a)+1; // size of input, string + terminator
//    defstream.next_in = (Bytef *)a; // input char array
//    defstream.avail_out = (uInt)sizeof(b); // size of output
//    defstream.next_out = (Bytef *)b; // output char array
//
//    // the actual compression work.
//    deflateInit(&defstream, Z_BEST_COMPRESSION);
//    deflate(&defstream, Z_FINISH);
//    deflateEnd(&defstream);
//
//    // This is one way of getting the size of the output
//    printf("Compressed size is: %lu\n", strlen(b));
//    printf("Compressed string is: %s\n", b);
//
//
//    printf("\n----------\n\n");
//
//
//    // STEP 2.
//    // inflate b into c
//    // zlib struct
//    z_stream infstream;
//    infstream.zalloc = Z_NULL;
//    infstream.zfree = Z_NULL;
//    infstream.opaque = Z_NULL;
//    // setup "b" as the input and "c" as the compressed output
//    infstream.avail_in = (uInt)((char*)defstream.next_out - b); // size of input
//    infstream.next_in = (Bytef *)b; // input char array
//    infstream.avail_out = (uInt)sizeof(c); // size of output
//    infstream.next_out = (Bytef *)c; // output char array
//
//    // the actual DE-compression work.
//    inflateInit(&infstream);
//    inflate(&infstream, Z_NO_FLUSH);
//    inflateEnd(&infstream);
//
//    printf("Uncompressed size is: %lu\n", strlen(c));
//    printf("Uncompressed string is: %s\n", c);
//
//
//    // make sure uncompressed is exactly equal to original.
//    assert(strcmp(a,c)==0);
//
//    return 0;
//}




//#include <iostream>
//#include <cstdlib>
//#include <pthread.h>
//
//using namespace std;
//
//#define NUM_THREADS     5
//
//void *PrintHello(void *threadid) {
//    long tid;
//    tid = (intptr_t) threadid;
//    cout << "Hello World! Thread ID, " << tid << endl;
//    pthread_exit(NULL);
//}
//
//int main () {
//    pthread_t threads[NUM_THREADS];
//    int rc;
//    int i;
//
//    for( i=0; i < NUM_THREADS; i++ ){
//        cout << "main() : creating thread, " << i << endl;
//        rc = pthread_create(&threads[i], NULL, PrintHello, (void *)(intptr_t)i);
//
//        if (rc){
//            cout << "Error:unable to create thread," << rc << endl;
//            exit(-1);
//        }
//    }
//
//    pthread_exit(NULL);
//}


//#include <iostream>
//#include <vector>
//#include "eigen3/Eigen/Dense"
//using namespace std;
//int main()
//{
//    Eigen::MatrixXf mat(3,4);
//    mat << 1, 2, 6, 9,
//            3, 1, 7, 2,
//            4, 5, 2, 1;
//
//    int numDim = 4;
//    vector<int> rows {0,1,2};
//    vector<int> max (4);
//    vector<int> min (4);
//
//    for (int d = 0; d < numDim; d++) {
//        max[d] = mat(rows[0], d);
//        min[d] = mat(rows[0], d);
//    }
//
//    for (int r = 1; r < rows.size(); r++) {
//        for (int d = 0; d < numDim; d++) {
//            if (max[d] < mat(rows[r],d)) {
//                max[d] = mat(rows[r],d);
//            }
//            if (min[d] > mat(rows[r],d)) {
//                min[d] = mat(rows[r],d);
//            }
//        }
//    }
//
//    for (int d = 0; d < numDim; d++) {
//        cout << d << ": " << max[d] - min[d] << endl;
//    }
//
//    int maxIndex = 0;
//    int maxValue = max[maxIndex] - min[maxIndex];
//    for (int d = 0; d < numDim; d++) {
//        int currentDiff = max[d] - min[d];
//        if (maxValue < currentDiff) {
//            maxValue = currentDiff;
//            maxIndex = d;
//        }
//    }
//
//    cout << maxIndex << ": " << maxValue << endl;
//
//
//
//
////    std::cout << "Column's maximum: " << std::endl
////              << mat.colwise().maxCoeff() << std::endl;
////
////    std::cout << "Column's minimum: " << std::endl
////              << mat.colwise().minCoeff() << std::endl;
////
////    std::cout << "Column's maximum - min: " << std::endl
////              << mat.colwise().maxCoeff() - mat.colwise().minCoeff() << std::endl;
////
////    Eigen::MatrixXf::Index maxIndex;
////    (mat.colwise().maxCoeff() - mat.colwise().minCoeff()).maxCoeff(&maxIndex);
////    std::cout << "Column's (maximum - min) plus: " << std::endl
////              <<  maxIndex << std::endl;
//
//}

//#include <iostream>
//
//#include <vector>
//
//using namespace std;
//
////class A {
////private:
////    int a;
////public:
////    A(int a) : a(a) {}
////};
////
////template <typename T>
////class B : public A {
////public:
////    B(int a, T v1) : A(a), v1(v1) {}
////    T getV1() const { return v1; }
////private:
////    T v1;
////};
////
////template <typename U>
////class C : public A {
////private:
////    U v2;
////public:
////    C(int a, U v2) : A(a), v2(v2) {}
////    U getV2() const { return v2; }
////};
//
//int main() {
//
////    vector<A> v;
////    v.push_back(new C<int>(10, 10));
//
//    cout << "Hello world!!" << endl;
//    cout << "0 " << (0-1)/2 << endl;
//    cout << "1 " << (1-1)/2 << endl;
//    cout << "2 " << (2-1)/2 << endl;
//    cout << "3 " << (3-1)/2 << endl;
//    cout << "4 " << (4-1)/2 << endl;
//    cout << "5 " << (5-1)/2 << endl;
//    cout << "6 " << (6-1)/2 << endl;
//
////
////    vector<int> v;
////    v.reserve(10);
////
////    v[5l] = 15;
////
////    cout << "Hello world!! 4 " << v[4] << endl;
////    cout << "Hello world!! 5 " << v[5] << endl;
////
////    for (int i: v) {
////        cout << "v: " << i << endl;
////    }
//    return 0;
//}
