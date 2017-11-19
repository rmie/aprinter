/* Copyright (c) 2015 Ambroz Bizjak and others
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef APRINTER_SERIAL_DISPLAY_MODULE_H
#define APRINTER_SERIAL_DISPLAY_MODULE_H

#include <aprinter/meta/ServiceUtils.h>
#include <aprinter/printer/PrinterMain.h>
#include <aprinter/printer/utils/JsonBuilder.h>
#include <aprinter/printer/utils/ModuleUtils.h>

namespace APrinter {

template <typename ModuleArg>
class SerialDisplayModule {
    APRINTER_UNPACK_MODULE_ARG(ModuleArg)

public:
    struct Object;

    using TheCommand = typename ThePrinterMain::TheCommand;
    using AxesList = typename ThePrinterMain::AxesList;
    using PhysVirtAxisMaskType = typename ThePrinterMain::PhysVirtAxisMaskType;
    using PhysVirtAxisHelperList = typename ThePrinterMain::PhysVirtAxisHelperList;

public:
    class CommandJsonBuilder : public AbstractJsonBuilder {

    public:
        // TODO: this is propably a mistake, but my template foo isn't good
        // enough to figure out how to get context otherwise
    	CommandJsonBuilder(const Context& c_, TheCommand* cmd) : c(c_) {
    		this->cmd = cmd;
    	}

    protected:
        void add_char (char ch) override
        {
        	cmd->reply_append_ch(c, ch);
        }


        void print (char const *fmt, uint32_t val) override
        {
        	cmd->reply_append_uint32(c, val);
        }

        void print (char const *fmt, double val) override
        {
        	cmd->reply_append_fp(c, val);
        }

    private:
        const Context& c;
        TheCommand* cmd;
    };

    static void init (Context c)
    {
        auto *o = Object::self(c);
    }

    static void deinit (Context c)
    {
    }

    static bool check_command (Context c, TheCommand *cmd)
    {
        if (cmd->getCmdNumber(c) == 408) {
        /* M408 S0
           {"status": "I","coords": {"axesHomed":[0, 0, 0],"extr":[0.000000,0.000000,0.000000],"xyz":[0.000000,0.000000,0.000000]},"currentTool":0,"params": {"atxPower":0,"fanPercent":[0.000000],"speedFactor":100,"extrFactors":[100,100,100]},"temps": {"heads": {"current": [22.083333,22.222221,178.291015],"active": [0.000000,0.000000,0.000000],"state": [1,1,1]}},"time":55160060}
           M408 S1
           {"status": "I","coords": {"axesHomed":[0, 0, 0],"extr":[0.000000,0.000000,0.000000],"xyz":[0.000000,0.000000,0.000000]},"currentTool":0,"params": {"atxPower":0,"fanPercent":[0.000000],"speedFactor":100,"extrFactors":[100,100,100]},"temps": {"heads": {"current": [22.083333,22.222221,178.291015],"active": [0.000000,0.000000,0.000000],"state": [1,1,1]}},"time":55165676}
           M408 S2
           {"status": "I","coords": {"axesHomed":[0, 0, 0],"extr":[0.000000,0.000000,0.000000],"xyz":[0.000000,0.000000,0.000000]},"currentTool":0,"params": {"atxPower":0,"fanPercent":[0.000000],"speedFactor":100,"extrFactors":[100,100,100]},"temps": {"heads": {"current": [22.083333,22.222221,178.291015],"active": [0.000000,0.000000,0.000000],"state": [1,1,1]}},"time":55213620,"coldExtrudeTemp":0,"coldRetractTemp":0.0,"geometry":"coreXY","name":"RepRap","tools":[{"number":0,"heaters":[1],"drives":[1]},{"number":1,"heaters":[1],"drives":[1]},{"number":2,"heaters":[1],"drives":[1]}]}
           M408 S3
           {"status": "I","coords": {"axesHomed":[0, 0, 0],"extr":[0.000000,0.000000,0.000000],"xyz":[0.000000,0.000000,0.000000]},"currentTool":0,"params": {"atxPower":0,"fanPercent":[0.000000],"speedFactor":100,"extrFactors":[100,100,100]},"temps": {"heads": {"current": [22.083333,22.222221,178.291015],"active": [0.000000,0.000000,0.000000],"state": [1,1,1]}},"time":55244837,"currentLayer":0,"extrRaw":[0,0,0],"fractionPrinted":0.000000,"firstLayerHeight":0.000000}
           M408 S4
           {"status": "I","coords": {"axesHomed":[0, 0, 0],"extr":[0.000000,0.000000,0.000000],"xyz":[0.000000,0.000000,0.000000]},"currentTool":0,"params": {"atxPower":0,"fanPercent":[0.000000],"speedFactor":100,"extrFactors":[100,100,100]},"temps": {"heads": {"current": [22.083333,22.222221,178.291015],"active": [0.000000,0.000000,0.000000],"state": [1,1,1]}},"time":55268085,"axisMins":[0,0,0],"axisMaxes":[370,300,150],"accelerations":[2000.000000,2000.000000,100.000000,5000.000000,5000.000000,5000.000000],"firmwareElectronics":"Arduino Due","firmwareName":"Repetier","firmwareVersion":"0.92.9","minFeedrates":[0,0,0,0,0,0],"maxFeedrates":[300.000000,300.000000,5.000000,50.000000,50.000000,50.000000]}
           M408 S5
           {"status": "I","coords": {"axesHomed":[0, 0, 0],"extr":[0.000000,0.000000,0.000000],"xyz":[0.000000,0.000000,0.000000]},"currentTool":0,"params": {"atxPower":0,"fanPercent":[0.000000],"speedFactor":100,"extrFactors":[100,100,100]},"temps": {"heads": {"current": [22.083333,22.222221,178.291015],"active": [0.000000,0.000000,0.000000],"state": [1,1,1]}},"time":55311726,"axisMins":[0,0,0],"axisMaxes":[370,300,150],"accelerations":[2000.000000,2000.000000,100.000000,5000.000000,5000.000000,5000.000000],"firmwareElectronics":"Arduino Due","firmwareName":"Repetier","firmwareVersion":"0.92.9","minFeedrates":[0,0,0,0,0,0],"maxFeedrates":[300.000000,300.000000,5.000000,50.000000,50.000000,50.000000]}
        */
            auto *o = Object::self(c);

            uint32_t s_param = cmd->get_command_param_uint32(c, 'S', 0);

            PhysVirtAxisMaskType extruder_axes;
            PhysVirtAxisMaskType move_axes;
            ListFor<AxesList>([&] APRINTER_TL(axis, (axis::IsExtruder ? extruder_axes : move_axes) |= axis::AxisMask()));

            CommandJsonBuilder b(c, cmd);
            AbstractJsonBuilder* builder = &b;

            builder->start().startObject()
            .addSafeKeyVal("active", JsonBool{o->active})
            .addObject(JsonSafeString{"coords"}, [&] {
                ThePrinterMain::get_json_axis_homing(c, builder);
                builder->addArray(JsonSafeString{"extr"}, [&] {
                    ListFor<PhysVirtAxisHelperList>([&] APRINTER_TL(axis,
                        if (axis::AxisMask & extruder_axes) builder->add(JsonDouble{axis::get_position(c)})
                ));})
                .addArray(JsonSafeString{"xyz"}, [&] {
                    ListFor<PhysVirtAxisHelperList>([&] APRINTER_TL(axis,
                        if (axis::AxisMask & move_axes) builder->add(JsonDouble{axis::get_position(c)})
                ));});
            })
            .endObject();
            cmd->reply_append_pstr(c, AMBRO_PSTR("\n"));
            cmd->finishCommand(c);
            return false;
        }
        return true;
    }
public:
    struct Object : public ObjBase<SerialDisplayModule, ParentObject, EmptyTypeList> {
        uint32_t dummy;
        bool active : 1;
        PhysVirtAxisMaskType axis_homing;
    };
};

struct SerialDisplayModuleService {
    APRINTER_MODULE_TEMPLATE(SerialDisplayModuleService, SerialDisplayModule)
};
/*
APRINTER_ALIAS_STRUCT_EXT(SerialDisplayModuleService, (
    APRINTER_AS_TYPE(PlatformAxesList),
    APRINTER_AS_TYPE(Params)
), (
    APRINTER_MODULE_TEMPLATE(SerialDisplayModuleService, SerialDisplayModule)
))
*/

} // namespace APrinter


#endif
