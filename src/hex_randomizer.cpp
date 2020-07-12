#include <algorithm>
#include <random>
#include <stdexcept>

#include "hex.hpp"

namespace hex
{

    uint8_t Randomizer::Random(uint8_t min, uint8_t max)
    {
        static std::default_random_engine generator(time(0));
        std::uniform_int_distribution<uint8_t> distribution(min, max);
        return distribution(generator);
    }

    uint8_t Randomizer::WeightedRandom(const std::vector<ValueAndWeight>& candidates)
    {
        size_t weightSum = 0;
        for(const auto& candidate : candidates)
            weightSum += candidate.second;

        static std::default_random_engine generator(time(0));
        std::uniform_int_distribution<size_t> distribution(0, weightSum - 1);
        size_t choice = distribution(generator);

        for(const auto& candidate : candidates)
        {
            if(choice < candidate.second)
                return candidate.first;
            else
                choice -= candidate.second;
        }

        throw std::runtime_error("Randomizer found an unexpected end");
    }

} // namespace hex
