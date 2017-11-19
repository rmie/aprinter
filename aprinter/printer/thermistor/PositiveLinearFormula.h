/*
 * Copyright (c) 2015 Ambroz Bizjak, Armin van der Togt and others
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

#ifndef APRINTER_AD849X_FORMULA_H
#define APRINTER_AD849X_FORMULA_H

#include <aprinter/meta/ServiceUtils.h>
#include <aprinter/math/FloatTools.h>
#include <aprinter/printer/Configuration.h>
#include <aprinter/base/ProgramMemory.h>

/*
 * AD849x - K-Type thermocouple amplifier, eg. https://www.adafruit.com/product/1778
 *     zero offset 1.25V, slope 5mV/K, ADC reference 3.3V
 *         ZeroAdcValue = 1.25V/3.3V        = 0.379
 *         AdcSlope     = (0.005V/K)/3.3V   = 1.515e-3 1/K
 *
 * MAX849x - K-Type thermocouple amplifier
 *     zero offset 0V, slope 5mV/K, ADC reference 3.3V
 *         ZeroAdcValue = 0
 *         AdcSlope     = 1.515e-3 1/K
 */

namespace APrinter {

template <typename Arg>
class PositiveLinearFormula {
    using Context      = typename Arg::Context;
    using Config       = typename Arg::Config;
    using FpType       = typename Arg::FpType;
    using Params       = typename Arg::Params;
    
public:
    static bool const NegativeSlope = false;
    
    template <typename Temp>
    static auto TempToAdc (Temp) -> decltype(Temp()*Config::e(Params::AdcSlope::i()) +
                                    Config::e(Params::ZeroAdcValue::i()));

    static FpType adcToTemp (Context c, FpType adc)
    {
        // prevent thermal runaway
        if (0 >= APRINTER_CFG(Config, CAdcSlope, c)) {
            Context::Printer::print_pgm_string(c, AMBRO_PSTR("//Error:AdcSlope must be positive\n"));
            return INFINITY;
        }
        if (!(adc <= APRINTER_CFG(Config, CAdcMaxTemp, c))) {
            return INFINITY;
        }
        if (!(adc >= APRINTER_CFG(Config, CAdcMinTemp, c))) {
            return -INFINITY;
        }
        return (adc - APRINTER_CFG(Config, CZeroAdcValue, c)) / APRINTER_CFG(Config, CAdcSlope, c);
    }

private:
    using CAdcMinTemp = decltype(ExprCast<FpType>(TempToAdc(Config::e(Params::MinTemp::i()))));
    using CAdcMaxTemp = decltype(ExprCast<FpType>(TempToAdc(Config::e(Params::MaxTemp::i()))));
    using CZeroAdcValue = decltype(ExprCast<FpType>(Config::e(Params::ZeroAdcValue::i())));
    using CAdcSlope = decltype(ExprCast<FpType>(Config::e(Params::AdcSlope::i())));

public:
    struct Object {};

    using ConfigExprs = MakeTypeList<CAdcMinTemp, CAdcMaxTemp, CZeroAdcValue, CAdcSlope>;
};

APRINTER_ALIAS_STRUCT_EXT(PositiveLinearFormulaService, (
    APRINTER_AS_TYPE(ZeroAdcValue),
    APRINTER_AS_TYPE(AdcSlope),
    APRINTER_AS_TYPE(MinTemp),
    APRINTER_AS_TYPE(MaxTemp)
), (
    APRINTER_ALIAS_STRUCT_EXT(Formula, (
        APRINTER_AS_TYPE(Context),
        APRINTER_AS_TYPE(ParentObject),
        APRINTER_AS_TYPE(Config),
        APRINTER_AS_TYPE(FpType)
    ), (
        using Params = PositiveLinearFormulaService;
        APRINTER_DEF_INSTANCE(Formula, PositiveLinearFormula)
    ))
))

}

#endif
