
#ifndef FX_IDENTID_H
#define FX_IDENTID_H

#include "FXPlatform.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

struct FXGUID
{
    FXINT64 nData64;
    FXINT64 nHead64;

    FXGUID()
    {
        nData64 = 0;
        nHead64 = 0;
    }

    FXGUID(FXINT64 nHeadData, FXINT64 nData)
    {
        nHead64 = nHeadData;
        nData64 = nData;
    }

    FXGUID(const FXGUID& xData)
    {
        nHead64 = xData.nHead64;
        nData64 = xData.nData64;
    }

    FXGUID& operator=(const FXGUID& xData)
    {
        nHead64 = xData.nHead64;
        nData64 = xData.nData64;

        return *this;
    }

    const FXINT64 GetData() const
    {
        return nData64;
    }

    const FXINT64 GetHead() const
    {
        return nHead64;
    }

    void SetData(const FXINT64 nData)
    {
        nData64 = nData;
    }

    void SetHead(const FXINT64 nData)
    {
        nHead64 = nData;
    }

    bool IsNull() const
    {
        return 0 == nData64 && 0 == nHead64;
    }

    bool operator == (const FXGUID& id) const
    {
        return this->nData64 == id.nData64 && this->nHead64 == id.nHead64;
    }

    bool operator != (const FXGUID& id) const
    {
        return this->nData64 != id.nData64 || this->nHead64 != id.nHead64;
    }

    bool operator < (const FXGUID& id) const
    {
        if (this->nHead64 == id.nHead64)
        {
            return this->nData64 < id.nData64;
        }

        return this->nHead64 < id.nHead64;
    }

    std::string ToString() const
    {
        return lexical_cast<std::string>(nHead64) + "-" + lexical_cast<std::string>(nData64);
    }

    bool FromString(const std::string& strID)
    {
        size_t nStrLength = strID.length();
        size_t nPos = strID.find('-');
        if (nPos == std::string::npos)
        {
            return false;
        }

        std::string strHead = strID.substr(0, nPos);
        std::string strData = "";
        if (nPos + 1 < nStrLength)
        {
            strData = strID.substr(nPos + 1, nStrLength - nPos);
        }

        try
        {
            nHead64 = lexical_cast<FXINT64>(strHead);
            nData64 = lexical_cast<FXINT64>(strData);

            return true;
        }
        catch (...)
        {
            return false;
        }
    }
};

#endif
