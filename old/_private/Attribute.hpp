#pragma once


template <typename T>
class Attribute
{
public:
    Attribute() = default;

    Attribute(T value)
    : mValue(value)
    {
    }

    Attribute(const Attribute&) = delete;
    Attribute(Attribute&&) = default;
    Attribute& operator=(const Attribute&) = delete;    
    Attribute& operator=(Attribute&&) = default;
    
    Attribute<T>& operator=(const T& value)
    {
        setValue(value);
        return *this;
    }  

    operator T() const
    {
        return getValue();
    }

    bool operator==(const TValue& value) const
    {
        return mValue == value;
    }

    bool operator!=(const T& value)  const
    {
        return mValue != value;
    }

    const T& operator*() const
    {
        return mValue;
    }
    const T* operator->() const
    {
        return &mValue;
    }

    void setValue(const T& value)
    {
        mValue = value;
        //ValueObserver<T>::notifyListeners(m_value);
    }

    const T& getValue() const
    {
        return mValue;
    }

    // ValueObserver<T>& getObserver()
    // {
    //     return *this;
    // }

private:
    T mValue;

};