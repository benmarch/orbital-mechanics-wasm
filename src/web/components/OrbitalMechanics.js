import Component, {html} from '../Component.js';
import './Header.js';
import './Router.js';

class OrbitalMechanics extends Component {
    static get template() {
        return html`
            <style>
                :host {
                    display: block;
                }
            </style>

            <om-header></om-header>
            <om-router></om-router>
        `;
    }
}

// upgrade after WASM loads
if (Module.OrbitalMechanics) {
    window.customElements.define('orbital-mechanics', OrbitalMechanics);
} else {
    Module.onRuntimeInitialized = () => {
        window.customElements.define('orbital-mechanics', OrbitalMechanics);
    }
}
