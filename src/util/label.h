#ifndef TEMP_VAR_LABEL_GENERATOR_H_
#define TEMP_VAR_LABEL_GENERATOR_H_

#include <string>
#include <random>

class TempVarLabelGenerator
{
public:
    static TempVarLabelGenerator &Instance()
    {
        static TempVarLabelGenerator instance;
        return instance;
    }

    std::string newGloablVarLabel()
    {
        return generateRandomString("global_var_label");
    }

    std::string newLabel()
    {
        return generateRandomString("__label_");
    }

private:
    TempVarLabelGenerator() : counter_(0) {}

    TempVarLabelGenerator(const TempVarLabelGenerator &) = delete;
    TempVarLabelGenerator &operator=(const TempVarLabelGenerator &) = delete;

    std::string generateRandomString(const std::string &prefix)
    {
        // static std::random_device rd;
        // static std::mt19937 gen(rd());
        // static std::uniform_int_distribution<> dis(0, 999);

        std::string result = prefix + "_" + std::to_string(counter_);
        ++counter_;
        return result;
    }

    int counter_;
};

std::string newGloablVarLabel();
std::string newLabel();
int newLocalTempVarID();
static std::string reg_temp_name(int ir_tempid)
{
    if (ir_tempid >= 0)
        return "r" + std::to_string(ir_tempid);
    else
        return "s" + std::to_string(~ir_tempid);
}

#endif // TEMP_VAR_LABEL_GENERATOR_H_
