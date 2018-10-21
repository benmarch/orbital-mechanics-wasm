const STORAGE_KEY = 'my-orbits';

class OrbitRepository {
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
        this.orbits[name] = orbit;
        this.storage.setItem(STORAGE_KEY, JSON.stringify(this.orbits));
    }
}