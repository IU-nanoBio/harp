/* file: image_dataset.h */
/*******************************************************************************
* Copyright 2017-2018 Intel Corporation
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
!  Content:
!    Auxiliary functions used in C++ neural networks samples
!******************************************************************************/

#ifndef _IMAGE_DATASET_H
#define _IMAGE_DATASET_H

#include <vector>
#include <fstream>
#include <stdint.h>
#include <stdexcept>

#include "daal_defines.h"

class DatasetReader
{
public:
    DatasetReader() { }
    virtual ~DatasetReader() { }

    virtual void read() = 0;
    virtual TensorPtr getTrainData() = 0;
    virtual TensorPtr getTrainGroundTruth() = 0;
    virtual TensorPtr getTestData() = 0;
    virtual TensorPtr getTestGroundTruth() = 0;
};


template<typename FPType>
class RGBChannelNormalizer
{
public:
    inline FPType operator()(FPType value) { return value / (FPType)255; }
};

template<typename FPType>
class DummyNormalizer
{
public:
    inline FPType operator()(FPType value) { return value; }
};


template<typename FPType, typename Normalizer = RGBChannelNormalizer<FPType> >
class ImageDatasetReader : public DatasetReader
{
public:
    size_t numberOfChannels;
    size_t objectHeight;
    size_t objectWidth;

protected:

    Normalizer _normalizer;
    SharedPtr<HomogenTensor<FPType> > _trainData;
    SharedPtr<HomogenTensor<FPType> > _trainGroundTruth;
    SharedPtr<HomogenTensor<FPType> > _testData;
    SharedPtr<HomogenTensor<FPType> > _testGroundTruth;

public:

    virtual ~ImageDatasetReader() { }

    virtual TensorPtr getTrainData() { return _trainData; }
    virtual TensorPtr getTrainGroundTruth() { return _trainGroundTruth; }
    virtual TensorPtr getTestData() { return _testData; }
    virtual TensorPtr getTestGroundTruth() { return _testGroundTruth; }

protected:

    ImageDatasetReader(size_t channelsNum, size_t height, size_t width) :
        numberOfChannels(channelsNum),
        objectHeight(height),
        objectWidth(width) { }

    virtual void allocateTensors()
    {
        size_t numberOfObjects = getNumberOfTrainObjects();
        size_t numberOfTestObjects = getNumberOfTestObjects();

        if (numberOfObjects > 0)
        {
            Collection<size_t> trainDataDims;
            trainDataDims.push_back(numberOfObjects);
            trainDataDims.push_back(numberOfChannels);
            trainDataDims.push_back(objectHeight);
            trainDataDims.push_back(objectWidth);
            _trainData = SharedPtr<HomogenTensor<FPType> >(
                             new HomogenTensor<FPType>(trainDataDims, Tensor::doAllocate, (FPType)0));

            Collection<size_t> trainGroundTruthDims;
            trainGroundTruthDims.push_back(numberOfObjects);
            trainGroundTruthDims.push_back(1);
            _trainGroundTruth = SharedPtr<HomogenTensor<FPType> >(
                                    new HomogenTensor<FPType>(trainGroundTruthDims, Tensor::doAllocate));
        }

        if (numberOfTestObjects > 0)
        {
            Collection<size_t> testDataDims;
            testDataDims.push_back(numberOfTestObjects);
            testDataDims.push_back(numberOfChannels);
            testDataDims.push_back(objectHeight);
            testDataDims.push_back(objectWidth);
            _testData = SharedPtr<HomogenTensor<FPType> >(
                            new HomogenTensor<FPType>(testDataDims, Tensor::doAllocate, (FPType)0));

            Collection<size_t> testGroundTruthDims;
            testGroundTruthDims.push_back(numberOfTestObjects);
            testGroundTruthDims.push_back(1);
            _testGroundTruth = SharedPtr<HomogenTensor<FPType> >(
                                   new HomogenTensor<FPType>(testGroundTruthDims, Tensor::doAllocate));
        }
    }

    virtual size_t getNumberOfTrainObjects() = 0;
    virtual size_t getNumberOfTestObjects() = 0;

    void normalizeBuffer(const uint8_t *buffer, FPType *normalized, size_t bufferSize)
    {
        for (size_t i = 0; i < bufferSize; i++)
        {
            normalized[i] = _normalizer((FPType)buffer[i]);
        }
    }

    inline size_t tensorOffset(size_t n, size_t k = 0, size_t h = 0, size_t w = 0)
    {
        return
            n * numberOfChannels * objectHeight * objectWidth +
            k * objectHeight * objectWidth +
            h * objectWidth +
            w;
    }
};

template<typename FPType, typename Normalizer = RGBChannelNormalizer<FPType> >
class DatasetReader_MNIST : public ImageDatasetReader<FPType, Normalizer>
{
private:

    const int DATA_MAGIC_NUMBER = 0x00000803;
    const int LABELS_MAGIC_NUMBER = 0x00000801;

    std::string _trainPathData;
    std::string _trainPathLabels;
    std::string _testPathData;
    std::string _testPathLabels;
    size_t _numOfTrainObjects;
    size_t _numOfTestObjects;

public:

    size_t originalObjectHeight;
    size_t originalObjectWidth;
    size_t margins;

public:

    DatasetReader_MNIST(size_t margin = 0) : ImageDatasetReader<FPType, Normalizer>(1, 28 + 2 * margin, 28 + 2 * margin),
        _numOfTrainObjects(0), _numOfTestObjects(0),
        originalObjectWidth(28), originalObjectHeight(28), margins(margin) { }

    virtual ~DatasetReader_MNIST() { }

    inline void setTrainBatch(const std::string &pathToBatchData,
                              const std::string &pathToBatchlabels, size_t numOfObjects)
    {
        _trainPathData = pathToBatchData;
        _trainPathLabels = pathToBatchlabels;
        _numOfTrainObjects = numOfObjects;
    }

    inline void setTestBatch(const std::string &pathToBatchData,
                             const std::string &pathToBatchLabels, size_t numOfObjects)
    {
        _testPathData = pathToBatchData;
        _testPathLabels = pathToBatchLabels;
        _numOfTestObjects = numOfObjects;
    }

    virtual void read()
    {
        this->objectWidth = originalObjectWidth + 2 * margins;
        this->objectHeight = originalObjectHeight + 2 * margins;
        this->allocateTensors();

        if (_numOfTrainObjects)
        {
            readBatchDataFile(_trainPathData, this->_trainData, _numOfTrainObjects);
            readBatchLabelsFile(_trainPathLabels, this->_trainGroundTruth, _numOfTrainObjects);
        }

        if (_numOfTestObjects > 0)
        {
            readBatchDataFile(_testPathData, this->_testData, _numOfTestObjects);
            readBatchLabelsFile(_testPathLabels, this->_testGroundTruth, _numOfTestObjects);
        }
    }

protected:

    virtual size_t getNumberOfTrainObjects() { return _numOfTrainObjects; }
    virtual size_t getNumberOfTestObjects() { return _numOfTestObjects; }

private:

    void readBatchDataFile(const std::string &batchPath, SharedPtr<HomogenTensor<FPType> > data, size_t numOfObjects)
    {
        std::ifstream batchStream(batchPath.c_str(), std::ifstream::in | std::ifstream::binary);
        FPType *dataRaw = data->getArray();
        readDataBatch(batchStream, dataRaw, numOfObjects);
        batchStream.close();
    }

    void readBatchLabelsFile(const std::string &batchPath, SharedPtr<HomogenTensor<FPType> > labels, size_t numOfObjects)
    {
        std::ifstream batchStream(batchPath.c_str(), std::ifstream::in | std::ifstream::binary);
        FPType *labelsRaw = labels->getArray();
        readLabelsBatch(batchStream, labelsRaw, numOfObjects);
        batchStream.close();
    }

    void readDataBatch(std::ifstream &stream, FPType *tensorData, size_t numOfObjects)
    {
        uint32_t magicNumber = readDword(stream);
        if (magicNumber != DATA_MAGIC_NUMBER)
        {
            throw std::runtime_error("Invalid data file format");
        }

        uint32_t numberOfImages = readDword(stream);
        if (numberOfImages < numOfObjects)
        {
            throw std::runtime_error("Number of objects too large");
        }

        uint32_t numberOfRows = readDword(stream);
        if (numberOfRows != originalObjectWidth)
        {
            throw std::runtime_error("Batch contains invalid images");
        }

        uint32_t numberOfColumns = readDword(stream);
        if (numberOfColumns != originalObjectHeight)
        {
            throw std::runtime_error("Batch contains invalid images");
        }

        size_t bufferSize = originalObjectWidth * originalObjectHeight;
        uint8_t *channelBuffer = new uint8_t[bufferSize];

        FPType *tensorDataPtr;
        for (size_t objectCounter = 0; objectCounter < numOfObjects && stream.good(); objectCounter++)
        {
            stream.read((char *)channelBuffer, bufferSize);
            tensorDataPtr = tensorData + this->tensorOffset(objectCounter);
            tensorDataPtr += margins * this->objectWidth;
            for (size_t i = 0; i < originalObjectHeight; i++)
            {
                tensorDataPtr += margins;
                this->normalizeBuffer(channelBuffer + i * originalObjectWidth, tensorDataPtr, originalObjectWidth);
                tensorDataPtr += originalObjectWidth + margins;
            }
        }

        delete[] channelBuffer;
    }

    void readLabelsBatch(std::ifstream &stream, FPType *labelsData, size_t numOfObjects)
    {
        uint32_t magicNumber = readDword(stream);
        if (magicNumber != LABELS_MAGIC_NUMBER)
        {
            throw std::runtime_error("Invalid data file format");
        }

        uint32_t numberOfItems = readDword(stream);
        if (numberOfItems < numOfObjects)
        {
            throw std::runtime_error("Number of objects too large");
        }

        char classNumber;
        for (size_t objectCounter = 0; objectCounter < numOfObjects && stream.good(); objectCounter++)
        {
            stream.get(classNumber);
            labelsData[objectCounter] = (FPType)classNumber;
        }
    }

    inline uint32_t readDword(std::ifstream &stream)
    {
        uint32_t dword;
        stream.read((char *)(&dword), sizeof(uint32_t));
        return endianDwordConversion(dword);
    }

    inline uint32_t endianDwordConversion(uint32_t dword)
    {
        return
            ((dword >> 24) & 0x000000FF) |
            ((dword >>  8) & 0x0000FF00) |
            ((dword <<  8) & 0x00FF0000) |
            ((dword << 24) & 0xFF000000);
    }

};

#endif