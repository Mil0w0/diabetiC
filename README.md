# diabetiC

This app is a journaling app for diabetic people who wants to keep trace of their glycemia aka blood sugar level;

# A user can : 
* create a profile (username, password, created_at)
* log in their profile 
    * define a target range glycemia;
    * write an entry in their journal : a glycemia value (int) with a NOW time stamp;
        * get a message if hypoglycemia or hyper glycemia + advices according to state;
    * consult their last X entries;
        * filter the entries to display : 
            * the X last ones ;
            * the last one of the day;
    * display the average glycemia they had for the last 
        * day, X days, weeks etc;
    * convert the glycemia from mg/dL to (other units);
    * exit their profile
* delete all the data stored on the db about them
* exit the app

# Before starting the app you can customize the "config.txt" file to change : 
 * the language of the app (en/fr) ; 
 * the units you want to use;
 * the user by default;
 * 
 * * * * * * 

Go to docs/ for more info about the app
    