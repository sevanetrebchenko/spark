
#ifndef SPARK_LAYER_H
#define SPARK_LAYER_H

#include <spark_pch.h>
#include <utilitybox/tools/global_defines.h>

namespace Spark {

    class Layer {
        public:
            explicit Layer(std::string name = "Layer");
            virtual ~Layer() = default;

            virtual void OnAttach();
            virtual void OnDetach();
            virtual void OnUpdate(float dt);
            virtual void OnImGuiRender();

            _NODISCARD_ const std::string& GetName() const;
        protected:
            std::string _name;
    };

}

#endif //SPARK_LAYER_H
