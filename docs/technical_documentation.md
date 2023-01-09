### Documentation Technique DiabetiC

## Contexte
I. Objectif	1

Cette application a pour but d'installer un suivi personnel de la glycémie d'une personne diabétique. Elle est la version numérique d'un journal tenu à la main au quotidien par les personnes diabétiques.

II. Cible
 * les personnes diabétiques de tous les types (T1, T2, gestationel et autres)
 * les personnes s'occupant d'un individu diabétique (animal de compagnie, enfants)


III.  Délais	
Decembre -> Février

## Points techniques	
I. Introduction 

Toute notre progression a été enregistrée via git et partagée via github, notre repo :  
    https://github.com/Mil0w0/diabetiC.git

II. Fonctionnalités avancées

    a. Enregistrer sa glycémie
        i. Coté structure en C
    Lorsque l'utilisateur est connecté, il peut enregistrer sa glycémie. 
    Pour cela, on va lui demander, la valeur de sa glycémie et un commentaire à ajouter (optionnel).
    Le tout va être stocké dans une structure Entry.
    
    struct Entry {
    Entry *next;
    double value; //the glycemia
    char * comment; // an optional comment about the glycemia
    char * taken_at ; //date when the glycemia was taken (can't be null)
    int entries; //number of entries of the journal
    }

    Entry est un noeud d'une liste chainée comprenant chaque glycémie rentrée par l'utilisateur à une date taken_at.
    Le nombre entries est incrémenté à chaque nouvelle entrées (Entry).
    La date taken_at n'est pas renseigné et est NULL par défaut car elle sera set grâce au NOW() du SQL.

    Value est un double qui peut prendre plusieurs formes selon l'unité utilisé pour exprimer la glycémie (nmol/L, mg/dL, g/L). 
    
    On vérifie donc que : 
    * value > 0 (et si virgule il existe, la remplacer par un . <=pas encore développé)
    * entries > 0
    * taille de comment < 256 caractères.

        ii. Coté Base de données. 
    Une fois que la structure a été créé, on est donc sûr qu'il n'y pas d'erreur dans les données, on peut donc envoyer chaque valeur dans la BDD.

    Une table ENTRY avec en colonne les mêmes attributs (sauf next) que la struct Entry est créé et on INSERT les valeurs récupérer dans la struct.
    A noter que taken_at est bien rempli grâce à NOW();   

    b. Récupérer les données des glycémies venant de la BDD lors de la connexion de l'utilisateur (hors première connexion)

    Lorsque l'utilisateur ferme l'appli, ses données sont donc sauvegardées grâce à une base de donnée SQLITE. Lorsqu'il revient et se connecte, pour afficher ses dernières glycémies ou ajouter une nouvelle entry, on recréé la liste chainée en utilisant les fonctions addEntry() et createEntry(). Elles ont été créés de sorte qu'on puisse les utiliser pour créer des noeuds depuis des inputs utilisateur ou qu'on récupère des données de la bdd puis qu'on crée des noeuds avec ces données.

    c. Alerte si glycémie hors de la cible.
    Si la glycémie que l'utilisateur entre dans son journal est en dehors de la cible alors un message d'alerte le lui indique et le conseil sur la démarche à suivre.
    Il est possible de désactiver cette option dans les settings ou le config.txt.

    d. Unités de la glycémie.

    e. Affichage des glycémies


III. Utilisation de la base de données

L'application utilise SQLite, une base de donnée, comme système de persistence des données.

* Choix de la bdd 
Nous avons décidé d'utiliser SQLite principalement car notre application n'a pas besoin de passer par un serveur. L'application fonctionne en local sur un appareil alors stocker les informations sur la machine du client est plus rapide bien que cela pose le problème de perte des données si le client perd l'accès à sa machine. De plus, nous n'avons pas besoin de stocker une large quantité de données ni d'envoyer plus d'une requête à la fois. Il n'est donc pas nécessaire d'utiliser une base de données comme MySql qui demanderait plus de ressources même si nous sommes plus familier avec.

* utilisation d'une requête - interraction entre le C et la BDD.
SQLite est développé en C, language également utilisé pour notre application, ce qui facilite son utilisation. La librairie sqlite utilisée n'est pas intégrée par défaut alors il faut l'intégrer à notre projet mais cela reste une manipulation peu complexe. c.f docs/how_to_use_sqlite3_C_interface.

* pourquoi le callback


## Mode d’utilisation :
I.          Back-office et administrateur	2
II.         Utilisateurs	3

## Synthèse des membres de l’équipe
● Loriane :
● Marjorie :
● Nino :
