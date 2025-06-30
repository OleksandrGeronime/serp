#pragma once

namespace serp
{

    class Invokable
    {
    public:
        virtual ~Invokable() = default;
        virtual void invoke() const = 0;
    };

} // namespace serp
