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
     * @brief Do some action
     */
    virtual void execute() = 0;
};


#endif
