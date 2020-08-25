
#ifndef SPARK_APPLICATION_H
#define SPARK_APPLICATION_H

#include <spark/core/layer.h>

namespace Spark {

    class Application {
        public:
            Application();
            ~Application();

            void PushLayer(Layer* layer);
            void PushOverlay(Layer* overlay);

            void Run();

        private:
            class ApplicationData;
            ApplicationData* _data;
    };

    // To be defined in the client.
    extern Application* CreateApplication();
}

#endif // SPARK_APPLICATION_H
