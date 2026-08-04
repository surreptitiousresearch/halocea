float pin_fraction(float value, float value0, float value1)
{
    if ( value0 >= value1 )
    {
        if ( value > value1 )
        {
            if ( value < value0 )
                return (value0 - value) / (value0 - value1);
            return 0.0f;
        }
        return 1.0f;
    }

    if ( value > value0 )
    {
        if ( value < value1 )
            return (value - value0) / (value1 - value0);
        return 1.0f;
    }
    return 0.0f;
}
