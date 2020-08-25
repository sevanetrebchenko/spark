
#ifndef SPARK_LAYER_H
#define SPARK_LAYER_H

#include <spark/core/core.h>

namespace Spark {

    class Layer {
        public:
            /**
             * Construct a new layer with a given name.
             * @param name - Name of the layer.
             */
            explicit Layer(std::string name = "Layer");

            /**
             * Clean up resources related to this layer.
             */
            virtual ~Layer() = default;

            /**
             * Function gets called when the layer is first attached.
             */
            virtual void OnAttach();

            /**
             * Function gets called when the layer is detached.
             */
            virtual void OnDetach();

            /**
             * Function gets called once per frame to update this layer.
             * @param dt - Timestep since the last frame.
             */
            virtual void OnUpdate(float dt);

            /**
             * Function gets called once per frame to draw ImGui elements related to this layer.
             */
            virtual void OnImGuiRender();

            /**
             * Get the name of this layer.
             * @return Layer name.
             */
            _NODISCARD_ const std::string& GetName() const;

        protected:
            std::string _name;
    };

}

#endif // SPARK_LAYER_H
