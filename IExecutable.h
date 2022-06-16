/**
 * @file IExecutable.h
 * @author Jan Wielgus
 * @brief 
 * @date 2020-07-31
 * 
 */

#ifndef IEXECUTALBE_H
#define IEXECUTALBE_H


class IExecutable
{
public:
    virtual ~IExecutable() {}

    /**
     * @brief Do some action.
     * @param dt_s Time between this and previous execution.
     */
    virtual void execute(float dt_s) = 0;
};


#endif
