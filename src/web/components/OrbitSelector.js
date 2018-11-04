import Component, {html} from '../Component.js';
import OrbitRepository from '../OrbitRepository.js';

class OrbitSelector extends Component {
    static get template() {
        return html`
            <style>

            </style>

            <div id="orbits"></div>
        `;
    }

    constructor() {
        super();

        this.maxOrbits = Number(this.getAttribute('max-orbits')) || 0;
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

    selectOrbit({target}) {
        const orbitName = target.dataset.name;

        this.selectedOrbits.push({
            name: orbitName,
            orbit: this.orbitRepository.getOrbitByName(orbitName)
        });

        if (this.maxOrbits) {
            this.selectedOrbits = this.selectedOrbits.slice(Math.max(this.selectedOrbits.length - this.maxOrbits, 0));
        }

        this.dispatchEvent(new CustomEvent('orbitselected', {detail: this.selectedOrbits}));
    }
}

window.customElements.define('orbit-selector', OrbitSelector);