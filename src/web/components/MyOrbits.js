import OrbitRepository from '../OrbitRepository.js';
import Component, {html} from '../Component.js';

class MyOrbits extends Component {
    static get template() {
        return html`
            <style>
                a {
                    color: inherit;
                    text-decoration: none;
                }

                a:hover {
                    text-decoration: underline;
                }
            </style>

            <ul id="orbitList"></ul>
        `;
    }

    constructor() {
        super();

        this.orbitRepository = new OrbitRepository();
    }

    connectedCallback() {
        this.renderOrbits();
    }

    getOrbits() {
        return this.orbitRepository.getOrbits();
    }

    renderOrbits() {
        const orbits = this.getOrbits();
        const listItems = Object.entries(orbits).map(([name, orbit]) => `<li><a href="#createorbit/${name}">${name}</a><om-button on:click="handleDelete" data-orbit="${name}">&times;</om-button></li>`);

        if (listItems.length) {
            this.orbitListElement.innerHTML = listItems.join('\n');
            this.parseTree();
        } else {
            this.orbitListElement.innerHTML = `<li>You do not have any saved orbits. <a href="#createorbit">Create one now.</a></li>`;
        }
    }

    handleDelete(event) {
        const orbitName = event.target.dataset.orbit;

        this.orbitRepository.deleteOrbit(orbitName);
        this.renderOrbits();
    }
}

window.customElements.define('my-orbits', MyOrbits);