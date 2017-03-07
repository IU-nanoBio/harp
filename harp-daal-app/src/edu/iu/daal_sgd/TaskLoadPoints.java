/*
 * Copyright 2013-2016 Indiana University
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * */

package edu.iu.daal_sgd;

import java.util.Arrays;
import java.lang.System;
import java.util.LinkedList;
import java.util.ArrayList;
import java.util.ListIterator;
import it.unimi.dsi.fastutil.ints.Int2ObjectOpenHashMap;
import java.nio.DoubleBuffer;
import java.nio.IntBuffer;

import com.intel.daal.data_management.data.*;
import com.intel.daal.data_management.data_source.*;
import com.intel.daal.data_management.data.NumericTable;
import com.intel.daal.data_management.data.HomogenNumericTable;

import com.intel.daal.services.*;
import com.intel.daal.algorithms.mf_sgd.*;

import com.intel.daal.services.DaalContext;

/**
 * @brief loading training dataset from a Harp table into a DAAl table 
 */
public class TaskLoadPoints implements Runnable {

    private int _th_id;
    private int _th_num;
    private LinkedList<VSet> _vSetList; 
    private int[] _wPos_daal_sets;
    private int[] _hPos_daal_sets;
    private double[] _val_daal_sets;

    private static DaalContext daal_Context = new DaalContext();

    //constructor
    TaskLoadPoints(
            int th_id, 
            int th_num,
            LinkedList<VSet> vSetList,
            int[] wPos_daal_sets,
            int[] hPos_daal_sets,
            double[] val_daal_sets
    )
    {
        _th_id = th_id;
        _th_num = th_num;
        _vSetList = vSetList;
        _wPos_daal_sets = wPos_daal_sets;
        _hPos_daal_sets = hPos_daal_sets;
        _val_daal_sets = val_daal_sets;
    }

    @Override
    public void run() {

        int itr_pos = 0;
        for(VSet vset : _vSetList)
        {

            int[] row_ids = new int[vset.getNumV()];
            Arrays.fill(row_ids, vset.getID()); 
            System.arraycopy(row_ids, 0, _wPos_daal_sets, itr_pos, vset.getNumV());
            System.arraycopy(vset.getIDs(), 0, _hPos_daal_sets, itr_pos, vset.getNumV());
            System.arraycopy(vset.getV(), 0, _val_daal_sets, itr_pos, vset.getNumV());

            itr_pos += vset.getNumV();

        }

    }


}











