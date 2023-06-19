# Project Hercules - analysing a pop up eye clinic with IoT

**An overview of the results can be seen in the [data / live](/data/live/) folder.**

## Project intro

Ophthalmology typically records the highest levels of attendances for specialist outpatient treatments with figures reaching 5.5 million visits for 2020-21. During the COVID pandemic patients missed standard check-ups and examinations creating a significant backlog of appointments. To expand capacity Moorfields Eye Hospital tested pop-up eye clinics in decentralised easy to access locations as an alternative to traditional appointments in a central London hospital to increase patient flow through these services.

The flexibility of the pop-up clinic supported easier analysis of different layouts in terms of patient flow and staff / patient safety through minimisation of infection risk throughout future waves of COVID-19. This project used Internet of Things (IoT) technologies to monitor patient flow on a per second basis through the clinic. This repository contains the code and data used to make spatio-temporal analysis of 4000 patients patient journeys to support informed clinic design decisions.

## Phase Data

The project ran over multiple phases (which equates to different clinic layouts and combinations of machines). The research started in October 2021. 

The [data](/data/) folder contains all the raw and cleaned data captured from the [Ubisense](http://www.ubisense.com) UWB IoT platform and notes on the timing of each phase. Overview statistics for each phase are also generated. The [exp1 folder](/data/exp1/) contains initial downloads when the ubisense system was set to a lower resolution - this data is no longer used. The [live folder](/data/live/) contains the working data.

The [data_import](/data_import/) folder contains the Python notebook scripts used to process the raw data (always the first step after raw download from Ubisense). The data output from these scripts are then saved in the [data folder](/data/) for cleaned input data (usually named something like P1_input.csv) or as inputs for  data visualisation (e.g. [data_vis_pde](/data_vis_pde/) folder). 

The [data_processing](/data_processing/) folder contains Python notebook scripts used to create a variety of box plots / charts describing the overall journey times. Thes scripts import the `Px_input.csv` files created in the `data_import` process.

## Floor plans + layout changes

Floor plans for each of the phase experiments are available in the [data_vis_pde](/data_vis_pde/) folder.

## Data Process Method

- Connect to Moorfields VPN and log in to MEHUBI server.

- Download data from Ubisense using the raw data download tool they have provided.

- Email csv file across to local machine.

- From [https://colab.research.google.com](https://colab.research.google.com) connect to GitHub Hercules project and open `importUbisense.ipynb` to import raw data from ubisense - cleans and removes any null values or non valid readings - it generates a `Px_input.csv` file as output which is downloaded and added to data repo. 

- Using Colab open up `patient_viewer_seconds.ipynb` import the `Px_input.csv` from above which in turn will generate a `Px_input_with_sec.csv` which is downloaded for use in the data_vis_pde scripts.

- Using Colab open up `journey_stats.ipynb` import the `Px_input.csv` from above which in turn will generate png's of box plots / charts with overview statistics.

- Final step is using the above data on the Processing sketch - `load_data` or `plots` - to explore the results.

## Data Formats

Files `Px_input.csv`

Data object with each step in dataset:
- id - usually letter(s) plus number e.g. R0123, C0124, G0123, Staff 10
- location - x,y (0,0 is bottom left! Values are floats)
- starttime - first datetime in "YYYY-MM-DD HH:MM:SS" format
- endtime - last datetime in "YYYY-MM-DD HH:MM:SS" format
- xlocation - x value from above
- ylocation - y value from above
- step_length - time in " x days HH:MM:SS" format

Files `Px_grouped_data.csv`

Data object with each row a unique patient:
- id - usually letter(s) plus number e.g. R0123, C0124, G0123, Staff 10
- starttime - first datetime in "YYYY-MM-DD HH:MM:SS" format
- endtime - last datetime in "YYYY-MM-DD HH:MM:SS" format
- visit_length - time in " x days HH:MM:SS" format
- dayofweek - Monday, Tuesday etc
- tod - time of day as "morning" or "afternoon"
- hour - integer
- condition - first letter from id
- visit_length_minutes - float of minutes from start to finish