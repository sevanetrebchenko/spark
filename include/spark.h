
#ifndef SPARK_SPARK_H
#define SPARK_SPARK_H

//-------------------------------
// For use by Spark applications.
//-------------------------------

//--- Core -------------------------------------------------------------------------------------------------------------
#include <spark/utility.h>
#include <spark/core/application.h>
#include <spark/core/layer.h>

//--- ECS --------------------------------------------------------------------------------------------------------------
#include <spark/ecs/components/all_component_types.h>
#include <spark/ecs/systems/base_component_system.h>

//--- Events -----------------------------------------------------------------------------------------------------------
#include <spark/events/types/application_events.h>
#include <spark/events/types/mouse_events.h>
#include <spark/events/types/key_events.h>
#include <spark/events/types/event.h>

//--- Renderer ---------------------------------------------------------------------------------------------------------


//--- Utility ----------------------------------------------------------------------------------------------------------
#include <spark/logger/logger.h>
#include <spark/logger/adapter/adapter.h>
#include <spark/logger/adapter/adapter_configuration.h>
#include <spark/logger/log_record.h>

#endif // SPARK_SPARK_H
