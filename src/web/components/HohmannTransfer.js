import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';

class HohmannTransfer extends Component {
    static get template() {
        return html`
            <div id="orbits"></div>

            <div id="selectedOrbits"></div>

            <div id="deltaV1"></div>
            <div id="deltaV2"></div>
            <div id="deltaVTotal"></div>
            <div id="timeOfFlight"></div>
        `;
    }

    constructor() {
        super();

        this.selectedOrbits = [];
        this.orbitRepository = new OrbitRepository();
    }

    connectedCallback() {
        this.renderOrbits();
    }

    renderOrbits() {
        const orbits = this.orbitRepository.getOrbits();

        const orbitList = Object.entries(orbits).map(([name, orbit]) => `<li>${name} <om-button data-name="${name}" on:click="selectOrbit">Select</om-button></li>`)

        this.orbitsElement.innerHTML = `<ul>${orbitList.join('')}</ul>`;
        this.parseTree();
    }

    renderSelectedOrbits() {
        const orbitFromName = this.selectedOrbits[0] ? this.selectedOrbits[0].name : '&lt;Select an Orbit&gt;'
        const orbitToName = this.selectedOrbits[1] ? this.selectedOrbits[1].name : '&lt;Select an Orbit&gt;'

        this.selectedOrbitsElement.innerHTML = `From: ${orbitFromName}, To: ${orbitToName}`;
    }

    selectOrbit({target}) {
        const orbitName = target.dataset.name;

        this.selectedOrbits.push({name: orbitName, orbit: this.orbitRepository.getOrbitByName(orbitName)});
        this.selectedOrbits = this.selectedOrbits.slice(0, 2);
        this.renderSelectedOrbits();

        if (this.selectedOrbits.length === 2) {
            this.setOrbits(this.selectedOrbits[0].orbit, this.selectedOrbits[1].orbit);
        }
    }

    setOrbits(orbitFrom, orbitTo) {
        const transfer = new Module.HohmannTransfer(orbitFrom, orbitTo);

        this.deltaV1Element.innerHTML = transfer.deltaV1 + ' km/s';
        this.deltaV2Element.innerHTML = transfer.deltaV2 + ' km/s';
        this.deltaVTotalElement.innerHTML = transfer.deltaVTotal + ' km/s'
        this.timeOfFlightElement.innerHTML = transfer.timeOfFlight + 's';
    }
}

window.customElements.define('hohmann-transfer', HohmannTransfer);