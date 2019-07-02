import os
import pandas as pd

NUCLIDE = 'V51'

ref_dir = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..', 'reference')
data = pd.read_csv(os.path.join(ref_dir, 'test31.csv'))

halflives = data[ (data['half_life'] > 1e4) & (data['half_life'] < 1e7)]
print(halflives[['cooling_time', 'activity', 'nuclide']])

nuclide_data = data[ data['nuclide'] == NUCLIDE]
print(nuclide_data)
