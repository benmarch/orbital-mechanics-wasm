import Component, {html} from '../Component.js';

class Button extends Component {
    static get template() {
        return html`
            <style>
                #button {
                    -webkit-appearance: none;
                    display: inline-block;
                    padding: 10px;
                    background-color: #fff;
                    border: var(--input-border);
                    cursor: pointer;

                    color: var(--text-color);
                    font-family: var(--font-family);
                    font-size: var(--font-size);
                    font-weight: var(--font-weight);
                    letter-spacing: var(--letter-spacing);
                    line-height: var(--line-height);
                }

                #button:hover {
                    background-color: #eee;
                }
            </style>

            <button id="button">
                <slot></slot>
            </button>
        `;
    }

    constructor() {
        super();
    }


}

window.customElements.define('om-button', Button);