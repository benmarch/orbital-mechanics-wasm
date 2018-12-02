import Component from '../Component.js';
import './MainMenu.js';
import './MyOrbits.js';
import './HohmannTransfer.js';
import './CreateOrbit.js';
import './PlaneChange.js';
import './CoplanarRendezvous.js';
import './CoorbitalRendezvous.js';
import './PatchedConicApproximation.js';
import './TimeOfFlightCalculator.js';
import './J2EffectCalculator.js'
import './LaunchWindowCalculator.js'
import './LaunchRequirementCalculator.js'
import './Reentry.js'
import './ThrustCalculationTools.js'
import './RocketStaging.js'

const routes = {
    'myorbits': 'my-orbits',
    'createorbit/?(?<name>[^/]*)': 'create-orbit',
    'hohmanntransfer': 'hohmann-transfer',
    'planechange': 'plane-change',
    'coplanarrendezvous': 'coplanar-rendezvous',
    'coorbitalrendezvous': 'coorbital-rendezvous',
    'pca': 'patched-conic-approximation',
    'tof': 'time-of-flight-calculator',
    'j2': 'j2-effect-calculator',
    'launchwindow': 'launch-window-calculator',
    'launchreq': 'launch-requirement-calculator',
    'reentry': 'reentry-calculator',
    'thrust': 'thrust-calculation-tools',
    'staging': 'rocket-staging',
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