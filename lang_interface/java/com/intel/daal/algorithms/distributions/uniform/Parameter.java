/* file: Parameter.java */
/*******************************************************************************
* Copyright 2014-2018 Intel Corporation
* All Rights Reserved.
*
* If this  software was obtained  under the  Intel Simplified  Software License,
* the following terms apply:
*
* The source code,  information  and material  ("Material") contained  herein is
* owned by Intel Corporation or its  suppliers or licensors,  and  title to such
* Material remains with Intel  Corporation or its  suppliers or  licensors.  The
* Material  contains  proprietary  information  of  Intel or  its suppliers  and
* licensors.  The Material is protected by  worldwide copyright  laws and treaty
* provisions.  No part  of  the  Material   may  be  used,  copied,  reproduced,
* modified, published,  uploaded, posted, transmitted,  distributed or disclosed
* in any way without Intel's prior express written permission.  No license under
* any patent,  copyright or other  intellectual property rights  in the Material
* is granted to  or  conferred  upon  you,  either   expressly,  by implication,
* inducement,  estoppel  or  otherwise.  Any  license   under such  intellectual
* property rights must be express and approved by Intel in writing.
*
* Unless otherwise agreed by Intel in writing,  you may not remove or alter this
* notice or  any  other  notice   embedded  in  Materials  by  Intel  or Intel's
* suppliers or licensors in any way.
*
*
* If this  software  was obtained  under the  Apache License,  Version  2.0 (the
* "License"), the following terms apply:
*
* You may  not use this  file except  in compliance  with  the License.  You may
* obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
*
* Unless  required  by   applicable  law  or  agreed  to  in  writing,  software
* distributed under the License  is distributed  on an  "AS IS"  BASIS,  WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
* See the   License  for the   specific  language   governing   permissions  and
* limitations under the License.
*******************************************************************************/

/**
 * @ingroup distributions_uniform
 * @{
 */
package com.intel.daal.algorithms.distributions.uniform;

import com.intel.daal.services.DaalContext;

/**
 * <a name="DAAL-CLASS-ALGORITHMS__DISTRIBUTIONS__UNIFORM__UNIFORMPARAMETER"></a>
 * @brief Class that specifies parameters of the uniform distribution
 */
public class Parameter extends com.intel.daal.algorithms.distributions.ParameterBase {
    /** @private */
    static {
        System.loadLibrary("JavaAPI");
    }

    /** @private */
    public Parameter(DaalContext context, long cObject) {
        super(context, cObject);
    }

    /**
     * Returns the left bound a of the interval from which the random values are selected
     * @return  Left bound of the interval
     */
    public double getA() {
        return cGetA(cObject);
    }

    /**
     * Sets the left bound a of the interval from which the random values are selected
     * @param a Left bound of the interval
     */
    public void setA(double a) {
        cSetA(cObject, a);
    }

    /**
     * Returns the right bound b of the interval from which the random values are selected
     * @return  Right bound of the interval
     */
    public double getB() {
        return cGetB(cObject);
    }

    /**
     * Sets the right bound b of the interval from which the random values are selected
     * @param b Right bound of the interval
     */
    public void setB(double b) {
        cSetB(cObject, b);
    }

    private native void cSetA(long cObject, double a);
    private native void cSetB(long cObject, double b);
    private native double cGetA(long cObject);
    private native double cGetB(long cObject);
}
/** @} */