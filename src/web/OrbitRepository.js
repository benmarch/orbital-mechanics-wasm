const STORAGE_KEY = 'my-orbits';

export default class OrbitRepository {
    constructor() {
        this.orbits = {};
        this.storage = window.localStorage;
        this.getOrbits();
    }

    getOrbits() {
        const parsedOrbits = JSON.parse(this.storage.getItem(STORAGE_KEY) || '{}');

        Object.entries(parsedOrbits).forEach(([name, orbitLike]) => {
            const orbit = new Module.Orbit();

            if (orbitLike.stateVectors) {
                orbit.updateFromStateVectors(orbitLike.stateVectors);
            } else if (orbitLike.elements) {
                orbit.updateFromOrbitalElements(orbitLike.elements);
            } else {
                return;
            }

            this.orbits[name] = orbit;
        })

        return this.orbits;
    }

    getOrbitByName(name) {
        return this.orbits[name];
    }

    saveOrbit(name, orbit) {
        this.orbits[name] = orbit;
        this.persist();
    }

    deleteOrbit(name) {
        if (this.getOrbitByName(name)) {
            Module._free(this.orbits[name]);
            delete this.orbits[name];
            this.persist();
        }
    }

    persist() {
        const orbitsToPersist = {};
        Object.entries(this.orbits).forEach(([name, orbit]) => {
            orbitsToPersist[name] = {
                elements: orbit.elements,
                stateVectors: orbit.stateVectors,
            };
        });

        this.storage.setItem(STORAGE_KEY, JSON.stringify(orbitsToPersist));
    }
}