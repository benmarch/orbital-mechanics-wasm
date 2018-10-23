import OrbitRepository from '../OrbitRepository.js';
import Component, {html} from '../Component.js';
import './Button.js';

class SaveOrbitButton extends Component {
    static get template() {
        return html`
            <style>
                :host {
                    display: inline-block;
                }
            </style>
            <om-button id="button" on:click="saveOrbit">Save Orbit</om-button>
        `;
    }

    constructor() {
        super();

        this.orbitRepository = new OrbitRepository();
    }

    saveOrbit() {
        const name = prompt('What would you like to call this orbit?');

        if (name) {
            this.orbitRepository.saveOrbit(name, this.orbit);
            this.dispatchEvent(new CustomEvent('orbitsaved', {name}));
        }
    }

    setOrbit(orbit) {
        this.orbit = orbit;
    }
}

window.customElements.define('save-orbit-button', SaveOrbitButton);