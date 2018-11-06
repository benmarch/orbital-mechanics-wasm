const STORAGE_KEY = 'my-orbits';

export default class OrbitRepository {
    constructor() {
        this.storage = window.localStorage;
        this.orbits = this.retrieveOrbits();
    }

    getOrbits() {
        return this.orbits;
    }

    getOrbitByName(name) {
        return this.orbits[name];
    }

    saveOrbit(name, orbit) {
        // make sure we're up-to-date first
        this.orbits = this.retrieveOrbits();

        this.orbits[name] = orbit;
        this.persist();
    }

    deleteOrbit(name) {
        // make sure we're up-to-date first
        this.orbits = this.retrieveOrbits();

        if (this.getOrbitByName(name)) {
            Module._free(this.orbits[name]);
            delete this.orbits[name];
            this.persist();
        }
    }

    retrieveOrbits() {
        const orbits = {};
        const parsedOrbits = JSON.parse(this.storage.getItem(STORAGE_KEY) || '{}');

        Object.entries(parsedOrbits).forEach(([name, orbitLike]) => {
            const orbit = new Module.Orbit(orbitLike.mu);

            if (orbitLike.stateVectors) {
                orbit.updateFromStateVectors(orbitLike.stateVectors);
            } else if (orbitLike.elements) {
                orbit.updateFromOrbitalElements(orbitLike.elements);
            } else {
                return;
            }

            orbits[name] = orbit;
        });

        return orbits;
    }

    persist() {
        const orbitsToPersist = {};
        Object.entries(this.orbits).forEach(([name, orbit]) => {
            orbitsToPersist[name] = {
                elements: orbit.elements,
                stateVectors: orbit.stateVectors,
                mu: orbit.mu,
            };
        });

        this.storage.setItem(STORAGE_KEY, JSON.stringify(orbitsToPersist));
    }
}
