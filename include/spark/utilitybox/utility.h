
#ifndef SPARK_UTILITY_BOX_UTILITY_H
#define SPARK_UTILITY_BOX_UTILITY_H

// Accepts files with both directory separators (/ or \)
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__))

#endif //SPARK_UTILITY_BOX_UTILITY_H
