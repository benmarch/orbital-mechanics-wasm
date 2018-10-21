import Component from '../Component.js';
import './MainMenu.js';
import './MyOrbits.js';
import './CreateOrbit.js';

const routes = {
    '': 'main-menu',
    'myorbits': 'my-orbits',
    'createorbit': 'create-orbit'
}

class Router extends Component {
    constructor() {
        super();

        window.addEventListener('hashchange', this.handleHashChange.bind(this), false);
    }

    connectedCallback() {
        this.handleHashChange();
    }

    handleHashChange() {
        const hash = window.location.hash.replace('#', '');

        if (routes[hash]) {
            if (this.routeNode) {
                this.shadowRoot.removeChild(this.routeNode);
            }

            this.routeNode = document.createElement(routes[hash]);
            this.shadowRoot.appendChild(this.routeNode);
        }
    }
}

window.customElements.define('om-router', Router);