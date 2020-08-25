
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

    // To be defined in the client.
    extern Application* CreateApplication();
}

#endif // SPARK_APPLICATION_H
