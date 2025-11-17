# 🍝 Philosophers [![nistanoj's 42 Philosophers Score](https://badge.nimon.fr/api/v2/cmgjygisn1482501pa9h6l9sg4/project/4594510)](https://github.com/Nimon77/badge42)

Une implémentation en C du célèbre problème du dîner des philosophes, utilisant le multithreading et les mutex pour illustrer les concepts de synchronisation et de gestion des ressources partagées.

## 📋 Table des matières

- [Utilisation](#utilisation)
- [Algorithme](#algorithme)

## Utilisation

### Syntaxe

```bash
./philo <nb_philosophes> <temps_mort> <temps_manger> <temps_dormir> <nb_repas>
```

### Paramètres

| Paramètre | Type | Description | Unité |
|-----------|------|-------------|-------|
| `nb_philosophes` | int | Nombre de philosophes (et de fourchettes) | - |
| `temps_mort` | int | Temps maximum sans manger avant de mourir | ms |
| `temps_manger` | int | Durée d'un repas | ms |
| `temps_dormir` | int | Durée du sommeil après manger | ms |
| `nb_repas` | int | Nombre de repas requis (optionnel) | - |

## Algorithme

### Principe de base

1. **Initialisation** : Création des threads (philosophes) et des mutex (fourchettes)
2. **Boucle principale** :
   - Prendre la fourchette de gauche
   - Prendre la fourchette de droite
   - Manger (durée : `temps_manger`)
   - Déposer les fourchettes
   - Dormir (durée : `temps_dormir`)
   - Penser
3. **Surveillance** : Thread séparé vérifiant si un philosophe est mort
4. **Terminaison** : Arrêt propre quand condition remplie ou philosophe mort

### Prévention du deadlock

L'implémentation utilise différentes stratégies possibles :

- Ordre d'acquisition des fourchettes
- Temporisation
- Limitation du nombre de philosophes mangeant simultanément
