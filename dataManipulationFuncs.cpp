#include "dataManipulationFuncs.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

/**
 * Parse data from a csv file. All datapoints in the csv are doubles
 *
 * @param[out] result data from the csv file. result[i] is the data from a column i.
 * @param[in] pathToFile path the csv
 * @param[in] numCol number of columns in the csv
 */
vector<vector<double>> parseData(const string pathToFile, size_t numCol)
{
    vector<vector<double>> result;
    for (size_t i = 0; i < numCol; i++)
    {
        result.push_back(vector<double>{});
    }

    ifstream ifs(pathToFile);
    if (ifs.fail())
    {
        cerr << "Could not open file " + pathToFile << endl;
        exit(2);
    }

    while (!ifs.eof())
    {
        string row;
        getline(ifs, row);

        stringstream unparsed(row);

        vector<string> parsedRow;

        for (size_t i = 0; i < numCol; i++)
        {
            getline(unparsed, parsedRow.at(i), ',');
            if (parsedRow.at(i) == "")
            {
                break;
            }
        }

        if (parsedRow.size() == numCol)
        {
            if (parsedRow.at(2) == "")
                break;
            result.at(0).push_back(stod(parsedRow.at(0)));
            result.at(1).push_back(stod(parsedRow.at(1)));
            result.at(2).push_back(stod(parsedRow.at(2)));
        }

        // vector<double> parsedRow;

        // for (int i = 0; i < row.length(); i++)
        // {
        //     if (row.at(i) == ',')
        //     {
        //         parsedRow.push_back(stod(row.substr(0, i)));
        //         row.erase(0, i + 1);
        //         i = 0;
        //     }
        // }
    }

    return result;
}

/**
 * Smooth data from csv using the sliding window technique.
   HINT: use getAvgNextNValues in this function so get that working first
 *
 * @param[out] result smoothed data. result[i] is the smoothed data from a column i.
 * @param[in] rawData the raw (not smoothed) data already gathered from parseData.
 * @param[in] windowSize the length of the window
 */
vector<vector<double>> smoothData(vector<vector<double>> &rawData, size_t windowSize)
{
    if (rawData.size() == 0)
    {
        cerr << "No data provided to the smooth data function" << endl;
        exit(2);
    }

    vector<vector<double>> result(rawData);

    // const vector<vector<double>> parsed = rawData;

    for (size_t i = 0; i < rawData.size(); i++)
    {
        size_t window = 3;
        if (rawData.size() - 1 - i < 3)
        {
            window = rawData.size() - i;
        }
        result.at(i).push_back(getAvgNextNValues(rawData[i], i, window));
    }
    return result;
}

/**
 * filter data from csv based on its age.
 *
 * @param[out] result data with that has ages in the range [minAge, maxAge]
 * @param[in] minAge the minAge as an int
 * @param[in] maxAge the minAge as an int
 */
vector<vector<double>> filterData(vector<vector<double>> &rawData, int minAge, int maxAge)
{
    if (rawData.size() == 0)
    {
        cerr << "No data provided to the filter data function" << endl;
        exit(2);
    }

    vector<vector<double>> result;
    for (size_t i = 0; i < rawData.size(); i++)
    {
        result.push_back(vector<double>{});
    }

    for (size_t i = 0; i < rawData.size(); i++)
    {
        for (size_t j = 0; j < rawData.at(i).size(); j++)
        {
            if (rawData.at(i).at(j) >= minAge && rawData.at(i).at(j) <= maxAge)
            {
                result.at(i).push_back(rawData.at(i).at(j));
            }
        }
    }

    return result;
}

/**
 * Compute the avg of the values in a sliding window given a starting index and window length.
    If the length of the window exceeds the length of the list, compute the avg of the remaining elements instead.
 *
 * @param[out] avg the avg of the sliding window. Don't need to round this.
 * @param[in] startIndex the starting index of the sliding window
 * @param[in] windowLength the length of the window
 * @param[in] v a vector of doubles

 * @Example 1:
    v = [1.0,2.0,3.0,4.0,5.5]
    startIndex = 2
    windowLength = 3
    avg = (3.0 + 4.0 + 5.5) / 3 = 4.16

 * @Example 2:
    v = [1.0,2.0,3.0,4.0,5.5]
    startIndex = 3
    windowLength = 3
    avg = (4.0 + 5.5) / 2 = 9.5
 */
double getAvgNextNValues(vector<double> const &v, size_t startIndex, size_t windowLength)
{
    double sum = 0.0;
    for (size_t i = 0; i < windowLength; i++)
    {
        sum = sum + v.at(startIndex + i);
    }
    return (sum / windowLength);
}