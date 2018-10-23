import Component from '../Component.js';
import './MainMenu.js';
import './MyOrbits.js';
import './CreateOrbit.js';

const routes = {
    'myorbits': 'my-orbits',
    'createorbit/?(?<name>[^/]*)': 'create-orbit',
    '': 'main-menu',
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
        const routeRegex = Object.keys(routes).find(routeRegex => hash.match(new RegExp(routeRegex)));

        if (routes[routeRegex]) {
            if (this.routeNode) {
                this.shadowRoot.removeChild(this.routeNode);
            }

            this.routeNode = document.createElement(routes[routeRegex]);

            const groups = hash.match(new RegExp(routeRegex)).groups || {};
            Object.keys(groups).forEach(groupName => groups[groupName] = window.decodeURIComponent(groups[groupName]));

            this.routeNode.routeData = {
                route: hash,
                regex: routeRegex,
                ...groups,
            }

            this.shadowRoot.appendChild(this.routeNode);
        }
    }
}

window.customElements.define('om-router', Router);