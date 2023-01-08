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
