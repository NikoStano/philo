# 🍝 Dining Philosophers

Une implémentation en C du célèbre problème des philosophes dîneurs, utilisant le multithreading et les mutex pour illustrer les concepts de synchronisation et de gestion des ressources partagées.

## 📋 Table des matières

- [Installation](#installation)
- [Utilisation](#utilisation)
- [Tests](#tests)
- [Algorithme](#algorithme)

## Installation

```bash
git clone https://github.com/NikoStano/philo.git
cd philo
make
```

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

### Format de sortie

```bash
[timestamp_ms] philosophe_X action
```

Actions possibles :

- `has taken a fork` - a pris une fourchette
- `is eating` - est en train de manger
- `is sleeping` - dort
- `is thinking` - réfléchit
- `died` - est mort

## Tests

Lancer la suite de tests complète :

```bash
make test
```

Cette commande exécute plusieurs scénarios de test pour valider :

- La détection correcte de la mort
- L'absence de data races
- Le respect des contraintes de temps
- La terminaison propre du programme

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
