/* file: outlier_detection_univariate.h */
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

/*
//++
//  Implementation of the interface for the univariate outlier detection algorithm
//  in the batch processing mode
//--
*/

#ifndef __OUTLIERDETECTION_UNIVARIATE_H__
#define __OUTLIERDETECTION_UNIVARIATE_H__

#include "algorithms/algorithm.h"
#include "data_management/data/numeric_table.h"
#include "services/daal_defines.h"
#include "outlier_detection_univariate_types.h"

namespace daal
{
namespace algorithms
{
namespace univariate_outlier_detection
{

namespace interface1
{
/**
 * @defgroup univariate_outlier_detection_batch Batch
 * @ingroup univariate_outlier_detection
 * @{
 */
/**
 * <a name="DAAL-CLASS-ALGORITHMS__UNIVARIATE_OUTLIER_DETECTION__BATCHCONTAINER"></a>
 * \brief Provides methods to run implementations of the univariate outlier detection algorithm.
 *        It is associated with the daal::algorithms::univariate_outlier_detection::Batch class
 *        and supports the methods of the univariate outlier detection in the %batch processing mode
 *
 * \tparam algorithmFPType  Data type to use in intermediate computations for the univariate outlier detection algorithm, double or float
 * \tparam method           Univariate outlier detection computation method, \ref daal::algorithms::univariate_outlier_detection::Method
 */
template<typename algorithmFPType, Method method, CpuType cpu>
class DAAL_EXPORT BatchContainer : public daal::algorithms::AnalysisContainerIface<batch>
{
public:
    /**
     * Constructs a container for the univariate outlier detection algorithm with a specified environment
     * in the batch processing mode
     * \param[in] daalEnv   Environment object
     */
    BatchContainer(daal::services::Environment::env *daalEnv);
    /** Default destructor */
    ~BatchContainer();
    /**
     * Computes the result of the univariate outlier detection algorithm in the batch processing mode
     *
     * \return Status of computations
     */
    virtual services::Status compute() DAAL_C11_OVERRIDE;
};

/**
 * <a name="DAAL-CLASS-ALGORITHMS__UNIVARIATE_OUTLIER_DETECTION__BATCH"></a>
 * \brief Runs the univariate outlier detection algorithm in the batch processing mode.
 * <!-- \n<a href="DAAL-REF-UNIVARIATE_OUTLIER_DETECTION-ALGORITHM">univariate outlier detection algorithm description and usage models</a> -->
 *
 * \tparam algorithmFPType  Data type to use in intermediate computations for the univariate outlier detection algorithm, double or float
 * \tparam method           univariate outlier detection computation method, \ref daal::algorithms::univariate_outlier_detection::Method
 *
 * \par Enumerations
 *      - \ref Method       Computation methods
 *      - \ref InputId      Identifiers of input objects
 *      - \ref ResultId     Identifiers of results
 */
template<typename algorithmFPType = DAAL_ALGORITHM_FP_TYPE, Method method = defaultDense>
class DAAL_EXPORT Batch : public daal::algorithms::Analysis<batch>
{
public:
    typedef algorithms::univariate_outlier_detection::Input  InputType;
    typedef algorithms::univariate_outlier_detection::Result ResultType;

    /** Default constructor */
    Batch()
    {
        initialize();
    }

    /**
     * Constructs an algorithm for computing univariate outlier detection by copying input objects and parameters
     * of another algorithm for computing univariate outlier detection
     * \param[in] other An algorithm to be used as the source to initialize the input objects
     *                  and parameters of the algorithm
     */
    Batch(const Batch<algorithmFPType, method> &other) : input(other.input)
    {
        initialize();
    }

    /**
    * Returns method of the algorithm
    * \return Method of the algorithm
    */
    virtual int getMethod() const DAAL_C11_OVERRIDE { return(int) method; }

    /**
     * Returns structure that contains computed univariate outlier detection results
     * \return Structure that contains computed univariate outlier detection results
     */
    ResultPtr getResult()
    {
        return _result;
    }

    /**
     * Registers user-allocated memory to store univariate outlier detection results
     * \param[in] result  Structure to store univariate outlier detection results
     *
     * \return Status of computations
     */
    services::Status setResult(const ResultPtr& result)
    {
        DAAL_CHECK(result, services::ErrorNullResult)
        _result = result;
        _res = _result.get();
        return services::Status();
    }

    /**
     * Returns a pointer to the newly allocated algorithm for computing univariate outlier detection
     * with a copy of input objects and parameters of this algorithm for computing univariate outlier detection
     * \return Pointer to the newly allocated algorithm
     */
    services::SharedPtr<Batch<algorithmFPType, method> > clone() const
    {
        return services::SharedPtr<Batch<algorithmFPType, method> >(cloneImpl());
    }

protected:
    virtual Batch<algorithmFPType, method> * cloneImpl() const DAAL_C11_OVERRIDE
    {
        return new Batch<algorithmFPType, method>(*this);
    }

    virtual services::Status allocateResult() DAAL_C11_OVERRIDE
    {
        services::Status s = _result->allocate<algorithmFPType>(&input, NULL, (int) method);
        _res = _result.get();
        return s;
    }

    void initialize()
    {
        Analysis<batch>::_ac = new __DAAL_ALGORITHM_CONTAINER(batch, BatchContainer, algorithmFPType, method)(&_env);
        _in = &input;
        _result.reset(new ResultType());
    }

public:
    InputType input; /*!< %Input data structure */

private:
    ResultPtr _result;
};
/** @} */
} // namespace interface1
using interface1::BatchContainer;
using interface1::Batch;

} // namespace univariate_outlier_detection
} // namespace algorithm
} // namespace daal
#endif