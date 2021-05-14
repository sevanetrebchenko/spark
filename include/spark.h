
#ifndef SPARK_SPARK_H
#define SPARK_SPARK_H

//-------------------------------
// For use by Spark applications.
//-------------------------------

//--- Core -------------------------------------------------------------------------------------------------------------
#include <spark/core/core.h>
#include <spark/core/application.h>
#include <spark/core/service_locator.h>
#include <spark/core/layer.h>

//--- ECS --------------------------------------------------------------------------------------------------------------
#include <spark/ecs/ecs_typedefs.h>
#include <spark/ecs/entities/entity_manager_interface.h>
#include <spark/ecs/entities/entity_callback_type.h>
#include <spark/ecs/components/all_components.h>
#include <spark/ecs/systems/base_component_system.h>

//--- Events -----------------------------------------------------------------------------------------------------------
#include <spark/events/event_hub_interface.h>
#include <spark/events/event_interactable_interface.h>
#include <spark/events/types/application_events.h>
#include <spark/events/types/mouse_events.h>
#include <spark/events/types/key_events.h>
#include <spark/events/types/event.h>

//--- Renderer ---------------------------------------------------------------------------------------------------------


//--- Utility ----------------------------------------------------------------------------------------------------------
#include <spark/utilitybox/logger/logging_hub_interface.h>
#include <spark/utilitybox/logger/logging_interface.h>
#include <spark/utilitybox/logger/adapter/adapter.h>
#include <spark/utilitybox/logger/adapter/adapter_config.h>
#include <spark/utilitybox/logger/log_message_severity.h>

#endif // SPARK_SPARK_H
