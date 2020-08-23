
#ifndef SPARK_APPLICATION_H
#define SPARK_APPLICATION_H

namespace Spark {

    class Application {
        public:
            Application();
            ~Application();

            void Run();

        private:
            class ApplicationData;
            ApplicationData* _data;
    };

}

#endif // SPARK_APPLICATION_H
