
#ifndef SPARK_LAYER_STACK_H
#define SPARK_LAYER_STACK_H

#include <core/layer.h>

namespace Spark {

    class LayerStack {
        public:
            LayerStack();
            ~LayerStack();

            void PushOverlay(Layer* overlay);
            void PopOverlay(Layer* overlay);

            void PushLayer(Layer* layer);
            void PopLayer(Layer* layer);

        private:
            class LayerStackData;
            LayerStackData* _data;
    };

}

#endif //SPARK_LAYER_STACK_H
