from datasets import load_dataset
import pandas as pd

#used this dataset to generate the csv file: https://huggingface.co/datasets/trentmkelly/US-food-nutrient-data/viewer/default/train?row=1
#will use Food Central API (https://fdc.nal.usda.gov/api-guide) to obtain product names as needed

foodData = load_dataset('trentmkelly/US-food-nutrient-data')


def parseFoodData(foodData, start, batchSize):
    df = pd.DataFrame(columns=['ID', 'COMPANY', 'CALS', 'WEIGHT'])

    for i in range(start, start + batchSize):
        ID = foodData['train'][i]['fdc_id']
        COMPANY = foodData['train'][i]['company']

        CALS = foodData['train'][i]['calories_kcal']
        WEIGHT = foodData['train'][i]['serving_size']

        row = pd.DataFrame({'ID' : [ID], 'COMPANY' : [COMPANY], 'CALS' : [CALS], 'WEIGHT': [WEIGHT]})
 
        df = pd.concat([df, row])
       

    return df


#add data in batches, batch size = 2000
#append df to csv
for i in range(0, 101000, 2000):
    currDf = parseFoodData(foodData, i, 2000)
    print("CURRENTLY: ", i)
    currDf.to_csv("food_data.csv", mode = 'a', index=False)



