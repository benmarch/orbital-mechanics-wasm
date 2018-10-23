const STORAGE_KEY = 'my-orbits';

export default class OrbitRepository {
    constructor() {
        this.storage = window.localStorage;
        this.getOrbits();
    }

    getOrbits() {
        this.orbits = JSON.parse(this.storage.getItem(STORAGE_KEY) || '{}');
        return this.orbits;
    }

    getOrbitByName(name) {
        return this.orbits[name];
    }

    saveOrbit(name, orbit) {
        this.orbits[name] = {
            elements: orbit.elements,
            stateVectors: orbit.stateVectors,
        };
        this.persist();
    }

    deleteOrbit(name) {
        if (this.getOrbitByName(name)) {
            delete this.orbits[name];
            this.persist();
        }
    }

    persist() {
        this.storage.setItem(STORAGE_KEY, JSON.stringify(this.orbits));
    }
}